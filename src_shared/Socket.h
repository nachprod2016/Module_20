#pragma once
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <string>

#define SIZE 1024 

class Socket 
{
public:
    std::string msg, endMsg, endPac;
    int fd_socket;
    char buffer[SIZE];
    size_t ptr;
    Socket() = delete;
    Socket(int);
    ~Socket() = default;
    void getMsg();
    void getString(std::string &);
    void sendMsg(std::string &);
    int getInstruction();
};