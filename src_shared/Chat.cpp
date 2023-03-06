#include "Chat.h"

Chat::Chat(std::string & user1, std::string & user2, std::string & adr) : user1_(user1), user2_(user2), adress(adr) {}

void Chat::addMessage(Message & m)
{
    messages.push_back(m);
}