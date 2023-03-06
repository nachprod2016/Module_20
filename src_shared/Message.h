#pragma once
#include <string>

struct Message
{
    const std::string  author_;
    const std::string _message;
    Message() = delete;
    Message(std::string &, std::string &&);
    Message(std::string &, std::string &);
    ~Message() = default;
};