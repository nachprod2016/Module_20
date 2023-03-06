#pragma once
#include <list>
#include "Message.h"

struct Chat
{
    std::string user1_;
    std::string user2_;
    std::string adress;
    std::list<Message> messages;
    Chat() = default;
    Chat(std::string &, std::string &, std::string &);
    ~Chat() = default;
    void addMessage(Message &);
    void showMessages();
    void writeMessage(std::string &);
};