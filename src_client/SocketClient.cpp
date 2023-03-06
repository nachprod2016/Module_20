#include "SocketClient.h"
#include <iostream>

SocketClient::SocketClient(int fd_soket) : Socket(fd_soket) {};

void SocketClient::getUsersList(std::set<std::string> & uList)
{
    bool f = true;
    std::string buf;
    while (f)
    {
        getMsg();
        while (true)
        {
            getString(buf);
            if (buf == endPac)
            {
                break;
            }
            if (buf == endMsg)
            {
                f = false;
                break;
            }
            uList.insert(buf);
            buf.clear();
        }
    }
}

int SocketClient::getChat(Chat & chat)
{
    std::string endChat = "---endChat---";
    getMsg();
    std::string buf;
    getString(buf);
    if (buf == endMsg)
    {
        return 1;
    }
    else
    {
        chat.user1_ = buf;
        getString(chat.user2_);
    }
    getString(chat.adress);
    while (true)
    {
        std::string author;
        getString(author);
        if (author == endChat)
        {
            return 0;
        }
        /*if (author == endMsg)
        {
            return 1;
        }*/
        if (author == endPac)
        {
            getMsg();
            continue;
        }
        std::string mess;
        getString(mess);
        Message message(author, mess);
        chat.addMessage(message);
    }
}

void SocketClient::sendMesgToSrvrBase(Chat & chat, Message & message)
{
    std::string msgToSrvrBase;
    msgToSrvrBase += "7\n" + chat.user1_ +'\n' + chat.user2_ + '\n' + chat.adress + '\n' + message.author_ + '\n' + message._message + '\n';
    sendMsg(msgToSrvrBase);
}