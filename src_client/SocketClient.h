#pragma once
#include <set>
#include "../src_shared/Socket.h"
#include "../src_shared/Chat.h"

class SocketClient : public Socket
{
public:
    SocketClient() = delete;
    SocketClient(int);
    ~SocketClient() = default;
    void getUsersList(std::set<std::string> &);
    int getChat(Chat & chat);
    void sendMesgToSrvrBase(Chat &, Message &);
};