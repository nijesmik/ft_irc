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

std::string parseLongMessage(std::stringstream &stream, std::string const &front) {
    std::stringstream ss;
    ss << front.substr(1);

    std::string rest;
    while (stream >> rest) {
        ss << ' ' << rest;
    }
    return ss.str();
}

Message Parser::parseMessage(std::stringstream &stream) {
    std::string param;
    std::vector<std::string> params;

    stream >> param;
    if (param.empty()) {
        return Message();
    }

    Message::command_t command = parseCommand(param);
    if (command == Message::UNKNOWN) {
        params.push_back(param);
    }

    while (stream >> param) {
        if (param[0] == ':') {
            param = parseLongMessage(stream, param);
        }
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
    if (command == "QUIT") {
        return Message::QUIT;
    }
    if (command == "JOIN") {
        return Message::JOIN;
    }
    if (command == "PART") {
        return Message::PART;
    }
    if (command == "TOPIC") {
        return Message::TOPIC;
    }
    if (command == "PRIVMSG") {
        return Message::PRIVMSG;
    }
    if (command == "INVITE") {
        return Message::INVITE;
    }
    if (command == "KICK") {
        return Message::KICK;
    }
    if (command == "MODE") {
        return Message::MODE;
    }
    if (command == "WHO") {
        return Message::WHO;
    }
    if (command == "BOT") {
        return Message::BOT;
    }
    return Message::UNKNOWN;
}

int Parser::parseLimit(std::string const &limit) {
    char *end;
    long parsed = std::strtol(limit.c_str(), &end, 10);
    if (*end != '\0' || parsed < 0 || parsed > std::numeric_limits<int>::max()) {
        throw std::invalid_argument("Error: invalid limit");
    }
    return static_cast<int>(parsed);
}