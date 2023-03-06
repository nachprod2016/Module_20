#include "UserData.h"

UserData::UserData(std::string & pass) : password(pass) {}

void UserData::addChat(std::string & key)
{
    std::string keyChat = key;
    chats.insert(key);
}