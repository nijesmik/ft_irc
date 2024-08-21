//
// Created by 김세진 on 8/12/24.
//

#include "Parser.hpp"
#include <iostream>

int Parser::parsePort(char *port) {
    char *end;
    long parsed = std::strtol(port, &end, 10);
    if (*end != '\0' || parsed < 0 || parsed > 65535) {
        throw std::invalid_argument("Error: invalid port");
    }
    return static_cast<int>(parsed);
}

Message Parser::parseMessage(std::stringstream &stream) {
    std::string param;
    std::vector<std::string> params;

    stream >> param;
    std::cout << "command: '" << param << "'" << std::endl;
    Message::command_t command = parseCommand(param);
    if (command == Message::UNKNOWN) {
        params.push_back(param);
    }

    while (stream >> param) {
        std::cout << "param: '" << param << "'" << std::endl;
        params.push_back(param);
    }
    return Message(command, params);
}

Message::command_t Parser::parseCommand(std::string const &command) {
    if (command == "PASS") {
        return Message::PASS;
    }
    if (command == "NICK") {
        return Message::NICK;
    }
    if (command == "USER") {
        return Message::USER;
    }
    if (command == "PING") {
        return Message::PING;
    }
    if (command == "JOIN") {
        return Message::JOIN;
    }
    return Message::UNKNOWN;
}