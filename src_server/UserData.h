#pragma once
#include <set>
#include <string>

struct UserData
{
    std::string password;
    std::set<std::string> chats;
    UserData(std::string &);
    ~UserData() = default;
    void addChat(std::string &);
};
