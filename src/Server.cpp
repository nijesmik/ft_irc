//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

Server::Server(char *port, char *password) :
        port(Parser::parsePort(port)), password(std::string(password)) {
}











