//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

const std::string Server::NETWORK_NAME = "ft_irc";
const std::string Server::VERSION = "1.0";

Server::Server(char *port, char *password) :
        eventController(Parser::parsePort(port), std::string(password)) {}

Server::~Server() {}

void Server::start() {
    std::cout << "Server started" << std::endl;
    while (true) {
        try {
            run();
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void Server::run() {
    int nev = eventController.pollEvents();
    switch (nev) {
        case -1: // error
            throw std::runtime_error("Error: event polling failed");
        case 0: // no events
            return;
        default:
            eventController.handleEvents(nev);
    }
}
