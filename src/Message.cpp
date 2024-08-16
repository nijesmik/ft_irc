//
// Created by 김세진 on 8/13/24.
//

#include "Message.hpp"

Message::Message() :
        command(Message::INVALID) {}

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