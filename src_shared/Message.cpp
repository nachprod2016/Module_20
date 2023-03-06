#include "Message.h"

Message::Message(std::string & author, std::string && message) : author_(author), _message(message){}

Message::Message(std::string & author, std::string & message) :author_(author), _message(message){}