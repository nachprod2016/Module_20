#pragma once
#include <map>
#include "../src_shared/Chat.h"
#include "UserData.h"
#include "../src_shared/Socket.h"

class SocketServer : public Socket
{
public:
    SocketServer() = delete;
    SocketServer(int);
    ~SocketServer() = default;
    void sendUsersList(std::map<std::string, UserData> &);
    void getMessage(std::string &, std::string &, std::string &, std::string &, std::string &);
    void sendChat(Chat &, std::string &);
};