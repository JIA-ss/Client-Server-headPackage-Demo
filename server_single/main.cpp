#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>//close()

#define HOST_IP "9.134.233.66"
#define HOST_PORT 6667
#define SOCKET_SENT_BUFFER 4 * 1024 * 1024
#define SOCKET_REC_BUFFER 4 * 1024 * 1024  //4M
#define ACK_SIZE 8
#define HOST_FILE_SAVE_PATH "/root/serverDemo/fileSave/"
#define HEAD_PACKAGE_SIZE 100

struct headPackage {
    std::string filename;
    off_t filesize;
    char* c_str;
    headPackage(const std::string& name,const off_t& size):filename(name),filesize(size){
        std::string temp = name + " " + std::to_string(size);
        c_str = new char[HEAD_PACKAGE_SIZE];
        memcpy(c_str,temp.c_str(),temp.size());
        if (temp.size() > HEAD_PACKAGE_SIZE)    std::cout << "包头大小超出预设范围" << std::endl;
        else {
            char* opt = c_str + temp.size();
            std::string plus(HEAD_PACKAGE_SIZE - temp.size(),' ');
            memcpy(opt,plus.c_str(),plus.size());
        }

    }
    ~headPackage(){
        delete c_str;
    }
};

std::string getNameByPath(const std::string& path) {
    int idx = 0;
    for (int i = path.size() - 1; i >= 0; i--) {
        if (path[i] == '/' || path[i] == '\\') {
            idx = i;
            break;
        }
    }
    return path.substr(idx + 1);
}

inline bool CHECK_ERR(int res, const char* errMsg) {
    if (res >= 0)   return true;
    perror(errMsg);
    return false;
}

headPackage getHeadPackage(const char* head) {
    std::string temp(head);
    int idx = 0;
    for(int i = 0; i < temp.size(); i++) {
        if (head[i] == ' ') {
            idx = i;
            break;
        }
    }
    int idx_ = 0;
    std::string name = temp.substr(0,idx);
    for(int i = idx + 1; i < temp.size(); i++) {
        if (head[i] == ' ') {
            idx_ = i;
            break;
        }
    }
    std::string size = temp.substr(idx + 1,idx_);
    headPackage res(name,stoll(size));
    return res;
}


int main() {
    std::cout << "=== server starting ===" <<std::endl;
    std::cout << "\tip:" << HOST_IP << std::endl;
    std::cout << "\tport:" << HOST_PORT <<std::endl;
    std::cout << "\tfile save path:" <<HOST_FILE_SAVE_PATH<<std::endl;


    //建立socket
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);

    int send_buffer_size = SOCKET_SENT_BUFFER;
    int rev_buffer_size = SOCKET_REC_BUFFER;
    int err_id = 1;
    err_id = setsockopt(socket_fd,SOL_SOCKET,SO_SNDBUF,&send_buffer_size,sizeof(socklen_t)); //设置发送缓冲区大小
    CHECK_ERR(err_id,"设置socket发送缓冲区错误\n");
    err_id = setsockopt(socket_fd,SOL_SOCKET,SO_RCVBUF,&rev_buffer_size,sizeof(socklen_t));  //设置接收缓冲区大小
    CHECK_ERR(err_id,"设置socket接收缓冲区错误\n");
    //输入协议类型、主机地址、端口
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;//inet_addr(std::string(HOST_IP).c_str());
    addr.sin_port = htons(HOST_PORT);
    //绑定并连接
    err_id = ::bind(socket_fd, (sockaddr *)&addr, sizeof(addr));
    CHECK_ERR(err_id,"*** *** socket建立连接失败 *** ***\n");

    err_id = ::listen(socket_fd,20);
    CHECK_ERR(err_id,"socket监听失败\n");
listenAgain:
    std::cout << "=== === 开始监听 === === \n";
    //客户端信息接收容器
    int client_socket_fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr_client;
    socklen_t addr_client_len = sizeof(addr_client);


    //接收客户端信息
    //【坑位1】*** *** 这里accept的返回值，是client的socket_fd值，而不是0或-1 *** ***
    client_socket_fd = ::accept(socket_fd,(sockaddr*)&addr_client,&addr_client_len);
    CHECK_ERR(err_id,"accept失败");
    std::cout << "== 检测到客户端连接，开始接收包头 ==" << std::endl;
    char* head_package_origin = new char[HEAD_PACKAGE_SIZE];
    char* head_package = head_package_origin;
    bzero(head_package,HEAD_PACKAGE_SIZE);

    off_t head_remain_size = HEAD_PACKAGE_SIZE;
    while (head_remain_size > 0) {
        //【坑位2】*** *** 这里recv的第一个fd参数，是client的socket_fd，而不是自身的socket_fd *** ***
        int res = ::recv(client_socket_fd,(void*)head_package,head_remain_size,0);
        head_remain_size -= res;
        head_package += res;
    }
    auto pack_head = getHeadPackage(head_package_origin);
    std::cout << "-- 包头接收完毕！-- \n" << "  filename: " << pack_head.filename << std::endl <<"  filesize: "<<pack_head.filesize<<std::endl;
    std::cout << "-- 发送ack包--\n" ;
    //回复ack
    char* ack_content = new char[ACK_SIZE];
    memcpy(ack_content,"get    ",ACK_SIZE);
    //【坑位3】 *** *** server向client发送数据时候，send的fd要填client的fd *** ***
    err_id = ::send(client_socket_fd,ack_content,ACK_SIZE,0);
    std::cout << "ack 发送长度: " << err_id << std::endl;
    CHECK_ERR(err_id,"ack发送失败");

    std::string filepath = HOST_FILE_SAVE_PATH + pack_head.filename;
    std::cout << "  save_path: " << filepath << std::endl << "-- 开始接收文件主体 --" <<std::endl;

    int file_save_fd = open(filepath.c_str(),O_RDWR|O_CREAT);
    truncate(filepath.c_str(),pack_head.filesize);

    //【坑位4】 *** *** mmap的时候，打开方式一定要检查好，我这了踩坑一直没拍到，一定一定注意，bad address 有可能是 只读映射，没有写入权限 *** ***
    void* file_save_mmap_addr = mmap(nullptr,pack_head.filesize,PROT_READ | PROT_WRITE, MAP_SHARED, file_save_fd, 0);
    char* opt = (char*)file_save_mmap_addr;
    off_t file_remain_size = pack_head.filesize;
    while (file_remain_size > 0) {
        int res = ::recv(client_socket_fd,opt,file_remain_size,0);
        if (!CHECK_ERR(res,"接收内容失败"))   {
            strerror(errno);
            std::cout << "异常退出程序！\n"; return 0;
        }
        file_remain_size -= res;
        opt += res;
    }
    std::cout << "-- 接收完毕,已断开连接 --" << std::endl;
    close(client_socket_fd);
    munmap(file_save_mmap_addr,pack_head.filesize);
    close(file_save_fd);
    goto listenAgain;

    return 0;


}
