//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

Server::Server(char *port, char *password) :
        password(std::string(password)),
        eventController(Parser::parsePort(port)) {}

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
            handleEvents(nev);
    }
}

void Server::handleEvents(int nev) {
    for (int i = 0; i < nev; i++) {
        Session *session = eventController.getEventSession(i);
        if (session) {
            Message message;
            session->read() >> message;
        }
    }
}
