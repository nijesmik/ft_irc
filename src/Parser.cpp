//
// Created by 김세진 on 8/12/24.
//

#include "Parser.hpp"

int Parser::parsePort(char *port) {
    char *end;
    long parsed = std::strtol(port, &end, 10);
    if (*end != '\0' || parsed < 0 || parsed > 65535) {
        throw std::invalid_argument("Error: invalid port");
    }
    return static_cast<int>(parsed);
}

Message Parser::parseMessage(std::stringstream &stream) {
    std::string command;
    std::string param;
    std::vector<std::string> params;

    stream >> command;
    while (!stream.eof()) {
        stream >> param;
        params.push_back(param);
    }
    return Message(parseCommand(command), params);
}

Message::command_t Parser::parseCommand(std::string const &command) {
    if (command == "PASS") {
        return Message::PASS;
    }
    return Message::INVALID;
}