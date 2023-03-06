#include "Server.h"
#include <iostream>
#include <unistd.h>
#include <iterator>

Server::Server()
{
    length = sizeof(clientAddress);
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_family = AF_INET;
    int listenStatus, bindStatus;
    fdServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (fdServerSocket == -1)
    {
        std::cout << "Сокет не был создан!" << std::endl;
        exit(1);
    }
    bindStatus = bind(fdServerSocket,  (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (bindStatus == -1)
    {
        std::cout << "Сокет не был привязан!" << std::endl;
        exit(1);
    }
    listenStatus = listen(fdServerSocket, 1);
    if (listenStatus == -1)
    {
        std::cout << "Сокет не слушает порт!" << std::endl;
        exit(1);
    }
    fdClientSocket = accept(fdServerSocket, (struct sockaddr*)&clientAddress, &length);
    close(fdServerSocket);
    sckt = new SocketServer(fdClientSocket);
}

void Server::listenPort()
{
    flag = true;
    while (flag)
    {
        sckt->getMsg();
        int instruction = sckt->getInstruction(); 
        runInstruction(instruction);  
    }
    close(fdClientSocket);
    delete sckt;
}

void Server::runInstruction(int instruction)
{
    switch (instruction)
    {
        case 1:
        {
            //Выполнить регистрацию
            registration();
            break;
        }
        case 2:
        {
            //Выполнить авторизацию
            authorization();
            break;
        }
        case 3:
        {
            //Завершить работу
            std::cout << "Клиент отключился, завешение работы сервера" << std::endl;
            flag = false;
            break;
        }
        case 4:
        {
            //Отправить список пользователей 
            sckt->sendUsersList(users);
            break;
        }
        case 5:
        {
            //Отправить чаты пользователя
            sendUserChats();
            break;
        }
        case 6:
        {
           //создать новый чат
           createNewChat();  
            break;
        }
        case 7:
        //добавить сообщение в чат
        addMessage();
        break;
    }
}

void Server::registration()
{
    std::string login, password, msg;
    sckt->getString(login);
    sckt->getString(password);
    if (users.find(login) != users.end())
    {
        std::cout << "Пользователь указал занятый логин при регистрации" << std::endl;
        msg += "0\n";
    }
    else
    {
        msg += "1\n";
        UserData newUser(password);
        std::pair<std::string, UserData> p = std::make_pair(login, newUser);
        users.insert(p);
        std::cout << "Пользователь " << login << " зарегестрирован" << std::endl;
    }
    sckt->sendMsg(msg);
}

void Server::authorization()
{
    std::string log, pass, msg;
    sckt->getString(log);
    sckt->getString(pass);
    if (users.find(log) == users.end())
    {
        std::cout << "Пользователь ввел несуществующий логин при авторизации" << std::endl;
        msg += "2\n";
        sckt->sendMsg(msg);
    }
    else if (users.find(log)->second.password != pass)
    {
        std::cout << "Пользователь ввел неверный пароль при авторизации" << std::endl;
        msg += "3\n";
        sckt->sendMsg(msg);
    }
    else
    {
        std::cout << "Пользователь " << log << " авторизовался" << std::endl;
        activeUser = log;
        msg += "4\n" + log + '\n';
        sckt->sendMsg(msg);
    }
}

void Server::createNewChat()
{
    std::string user1, user2, adress, author, message;
    sckt->getMessage(user1, user2, adress, author, message);
    Message newMessage(author, message);
    Chat newChat(user1, user2, adress);
    newChat.addMessage(newMessage);
    std::pair<std::string, Chat> p = std::make_pair(adress, newChat);
    chats.insert(p);
    users.find(user1)->second.addChat(adress);
    users.find(user2)->second.addChat(adress);
}

void Server::sendUserChats()
{
    UserData & ud = users.find(activeUser)->second;
    std::set<std::string>::iterator begin = ud.chats.begin();
    std::set<std::string>::iterator end = ud.chats.end();
    for (begin; begin != end; begin++)
    {
        Chat & chat = chats.find(*begin)->second;
        sckt->sendChat(chat, activeUser);        
    }
    std::string answer = sckt->endMsg + '\n';
    sckt->sendMsg(answer);
}

void Server::addMessage()
{
    std::string user1, user2, adress, author, mess;
    sckt->getMessage(user1, user2, adress, author, mess);
    Message message(author, mess);
    Chat & chat = chats.find(adress)->second;
    chat.addMessage(message);
}

void Server::downloadData()
{
    std::cout << "Загрузка данных" << std::endl;
    std::ifstream download;
    download.open("./src_server/DataUsers.txt");
    if (download.is_open() == 0)
    {
        std::cout << "Данные не были обнаружены, созданы новые файлы для хранения данных" << std::endl;
        system("touch ./src_server/DataUsers.txt ./src_server/DataChats.txt");
        system("chmod 700 ./src_server/DataUsers.txt");
        system("chmod 700 ./src_server/DataChats.txt");
        download.close();
        return;
    }
    std::string endObj = "-----------------------------";
    while (true)
    {
        std::string log, pass;
        std::getline(download, log);
        if (log.size() == 0)
        {
            break;
        }
        std::getline(download, pass);
        UserData newUser(pass);
        while (true)
        {
            std::string buf;
            std::getline(download, buf);
            if (buf == endObj)
            {
                break;
            }
            newUser.addChat(buf);
        }
        std::pair<std::string, UserData> p = std::make_pair(log, newUser);
        users.insert(p);
    }
    download.close();
    download.open("./src_server/DataChats.txt");
    while (true)
    {
        std::string adr1, user1, user2, adr2;
        std::getline(download, adr1);
        if (adr1.size() == 0)
        {
            break;
        }
        std::getline(download, user1);
        std::getline(download, user2);
        std::getline(download, adr2);
        Chat newChat(user1, user2, adr2);
        while (true)
        {
            std::string author, mess;
            std::getline(download, author);
            if (author ==  endObj)
            {
                break;
            }
            std::getline(download, mess);
            Message newMessage(author, mess);
            newChat.addMessage(newMessage);
        }
        std::pair<std::string, Chat> p = std::make_pair(adr1, newChat);
        chats.insert(p); 
    }
    download.close();
    std::cout << "Данные были загружены!" << std::endl;
}

void Server::saveData()
{
    std::cout << "Сохранение данных" << std::endl;
    std::ofstream save;
    std::string endObj = "-----------------------------";
    save.open("./src_server/DataUsers.txt");
    std::map<std::string, UserData>::iterator begin = users.begin();
    std::map<std::string, UserData>::iterator end = users.end();
    for (begin; begin != end; begin++)
    {
        std::string buf;
        buf += begin->first +'\n' + begin->second.password + '\n';
        std::set<std::string>::iterator start = begin->second.chats.begin();
        std::set<std::string>::iterator finish = begin->second.chats.end();
        for (start; start != finish; start++)
        {
            buf += *start + '\n';
        }
        buf += endObj + '\n';
        save << buf;
    }
    save.close();
    save.open("./src_server/DataChats.txt");
    std::map<std::string, Chat>::iterator begin_ch = chats.begin();
    std::map<std::string, Chat>::iterator end_ch = chats.end();
    for (begin_ch; begin_ch != end_ch; begin_ch++)
    {
        std::string buf;
        buf += begin_ch->first +'\n' + begin_ch->second.user1_ + '\n';
        buf += begin_ch->second.user2_ + '\n' + begin_ch->second.adress + '\n';
        std::list<Message>::iterator start = begin_ch->second.messages.begin();
        std::list<Message>::iterator finish = begin_ch->second.messages.end();
        for (start; start != finish; start++)
        {
            buf += start->author_ + '\n' + start->_message + '\n';
        }
        buf += endObj + '\n';
        save << buf;
    }
    save.close();
}