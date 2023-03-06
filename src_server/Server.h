#pragma once
#include <fstream>
#include <map>
#include "UserData.h"
#include "SocketServer.h"
#include "../src_shared/Chat.h"

#define PORT 7777

class Server
{
private:
    int fdServerSocket, fdClientSocket;
    struct sockaddr_in  serverAddress, clientAddress;
    socklen_t length;
    bool flag;
    std::string activeUser;
    std::map<std::string, UserData> users;
    std::map<std::string, Chat> chats; 
    SocketServer* sckt;
public:
    Server();
    ~Server() = default;
    void listenPort();
    void runInstruction(int instruction);
    void registration();
    void authorization();
    void createNewChat();
    void sendUserChats();
    void addMessage();
    void downloadData();
    void saveData();
};
