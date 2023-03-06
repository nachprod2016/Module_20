#pragma once
#include <set>
#include "SocketClient.h"
#include "User.h"
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define PORT 7777

class Client
{
private:
    bool flag;
    int serverSocket;
    struct sockaddr_in serverAddress;
    User* user;
    SocketClient* sckt;
    void createUser();
    void showUsersList();
    void addNewChat();
public:
    Client();
    ~Client() = default;
    void work();
    void startMenu();
    void userSession();
    void getUserChats();
    void showContacts();
    void writeMessage();
    void showMessChat(Chat &);
    void writeAll();
    bool inputMessage(std::string &);
};