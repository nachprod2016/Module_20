#include "SocketServer.h"
#include <iostream>

SocketServer::SocketServer(int fd_soket) : Socket(fd_soket) {}

void SocketServer::sendUsersList(std::map<std::string, UserData> & users)
{
    std::string uList;
    size_t size = 0;
    std::map<std::string, UserData>::iterator begin = users.begin();
    std::map<std::string, UserData>::iterator end = users.end();
    for (begin; begin != end; begin++)
    {
        if (size + begin->first.size() < SIZE - endPac.size() - 1)
        {
            uList += begin->first + '\n';
            size += begin->first.size() + 1;
        }
        else
        {
            uList += endPac + '\n';
            sendMsg(uList);
            size = begin->first.size() + 1;
            uList += begin->first + '\n';
        }
    }
    uList += endMsg + '\n';
    sendMsg(uList);
}

void SocketServer::getMessage(std::string & user1, std::string & user2, std::string & adress, std::string & author, std::string & message)
{
    getString(user1);
    getString(user2);
    getString(adress);
    getString(author);
    getString(message);
}

void SocketServer::sendChat(Chat & chat, std::string & activeUser)
{
    std::string endChat = "---endChat---";
    std::string msg;
    std::string user1, user2, adress, buf;
    user1 = chat.user1_;
    user2 = chat.user2_;
    adress = chat.adress;
    msg += user1 + '\n' + user2 + '\n' + adress + '\n';
    size_t size = msg.size();
    std::list<Message>::iterator begin = chat.messages.begin();
    std::list<Message>::iterator end = chat.messages.end();
    for (begin; begin != end; begin++)
    {
        buf += begin->author_ + '\n' + begin->_message + '\n';
        if (size + buf.size() < SIZE - endPac.size() - 1)
        {
            msg += buf;
            size += buf.size();
            buf.clear();
        }
        else
        {
            msg += endPac + '\n';
            sendMsg(msg);
            msg += buf;
            size = buf.size();
            buf.clear();
        }
    }
    msg += endChat + '\n';
    sendMsg(msg);
}