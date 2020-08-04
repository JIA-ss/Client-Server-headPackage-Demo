cmake_minimum_required(VERSION 2.8)
project(server_single)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)
add_executable(server_single main.cpp)