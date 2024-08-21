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

Message::command_t Message::getCommand() const {
    return this->command;
}

std::string Message::getParam(size_t index) const {
    if (index >= this->params.size()) {
        return "";
    }
    return this->params[index];
}

std::string Message::getParam() const {
    return this->getParam(0);
}

std::string Message::getParamsAll(int index, char ignore) const {
    std::stringstream ss;
    std::string param;

    while (!(param = getParam(index)).empty()) {
        ss << param;
        index++;
    }

    if (ignore && ss.peek() == ignore) {
        ss.ignore(1);
    }

    return ss.str();
}