//
// Created by 김세진 on 8/13/24.
//

#include "Message.hpp"

Message::Message() :
        command(Message::UNKNOWN) {}

Message::Message(Message::command_t command, std::vector<std::string> const &params) :
        command(command),
        params(params) {}

Message::~Message() {}

Message &Message::operator=(Message const &other) {
    if (this == &other) {
        return *this;
    }
    this->command = other.command;
    this->params = other.params;
    return *this;
}

size_t Message::getParamSize() const {
    return this->params.size();
}

Message::command_t Message::getCommand() const {
    return this->command;
}

std::string Message::getParam(size_t index) const {
    if (index >= this->params.size()) {
        return std::string();
    }
    return this->params[index];
}

std::string Message::getParam() const {
    return this->getParam(0);
}

std::vector<std::string> Message::splitParam(int index, char delimiter) const {
    std::vector<std::string> splited;
    std::stringstream ss(getParam(index));
    std::string str;

    while (std::getline(ss, str, delimiter)) {
        splited.push_back(str);
    }
    return splited;
}