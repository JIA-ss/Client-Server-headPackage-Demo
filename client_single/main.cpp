#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>   //查看文件大小
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>//close()
using namespace std;

#define HOST_IP "9.134.233.66"
#define HOST_PORT 6667
#define SOCKET_SENT_BUFFER 4 * 1024 * 1024
#define SOCKET_REC_BUFFER 4 * 1024 * 1024  //4M
#define ACK_SIZE 8
#define HEAD_PACKAGE_SIZE 100

struct headPackage {
    string filename;
    off_t filesize;
    char* c_str;
    headPackage(const string& name,const off_t& size):filename(name),filesize(size){
        string temp = name + " " + to_string(size);
        c_str = new char[HEAD_PACKAGE_SIZE];
        memcpy(c_str,temp.c_str(),temp.size());
        if (temp.size() > HEAD_PACKAGE_SIZE)    cout << "包头大小超出预设范围" << endl;
        else {
            char* opt = c_str + temp.size();
            string plus(HEAD_PACKAGE_SIZE - temp.size(),' ');
            memcpy(opt,plus.c_str(),plus.size());
        }

    }
    ~headPackage(){
        delete c_str;
    }
};

string getNameByPath(const string& path) {
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

int main() {
    cout << "=== client starting ===" <<endl;
    string filepath = "";
    cout << "please input file path：" << endl;
    cin >> filepath;
    int file_fd = open(filepath.c_str(),O_RDONLY);  //只读
    struct stat stat_;
    fstat(file_fd,&stat_);
    cout << "file size: " << stat_.st_size << endl;


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
    addr.sin_addr.s_addr = inet_addr(string(HOST_IP).c_str());
    addr.sin_port = htons(HOST_PORT);
    //建立连接
    err_id = connect(socket_fd,(sockaddr *)&addr,sizeof(addr));
    CHECK_ERR(err_id,"socket建立连接失败\n");


    cout << "== start send head package ==\n";
    //定义包头，告诉服务器文件信息
    headPackage head(getNameByPath(filepath),stat_.st_size);
    char* tran = head.c_str;
    //发送包头
    off_t head_size = strlen(tran);
    off_t remain_size = head_size;
    while (remain_size > 0){
        off_t res = send(socket_fd,(void*)tran,head_size,0);
        if(!CHECK_ERR(res,"发送包头失败")) {
            break;
        }
        tran += res;
        remain_size -= res;
    }
    cout << "== send head package succeed :" << head.c_str << endl;

    //接收ack
    cout << "-- recv ack --\n";
    char* ack_buff_ = new char[ACK_SIZE];
    char* ack_buff = ack_buff_;
    int ack_remain = ACK_SIZE;
    while (ack_remain > 0) {
        int res = recv(socket_fd,ack_buff,ACK_SIZE,0);
        if(!CHECK_ERR(res,"ack接收失败")) {
            std::cout << "程序异常退出\n";
            return 0;
        }

        ack_remain -= res;
        ack_buff += res;
    }
    cout << "ack :" << ack_buff_ << endl;
    cout << "-- ack over --\n";

    //发送大文件
    void* file_mmap_addr = mmap(0,stat_.st_size,PROT_READ, MAP_SHARED,file_fd,0);
    char* opt = (char*)file_mmap_addr;
    off_t file_remain_size = stat_.st_size;
    while (file_remain_size > 0) {
        off_t res = send(socket_fd,(void*)opt,file_remain_size,0);
        if (!CHECK_ERR(res,"发送文件主体失败")) break;
        opt += res;
        file_remain_size -= res;
    }
    cout << "=== send over ===" << endl;
    cout << "send size: " << stat_.st_size - file_remain_size << endl;
    munmap(file_mmap_addr,stat_.st_size);
    close(file_fd);
    close(socket_fd);
    return 0;
}
