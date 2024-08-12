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
