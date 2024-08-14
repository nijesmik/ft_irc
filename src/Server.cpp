//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

Server::Server(char *port, char *password) :
        password(std::string(password)),
        eventManager(connection) {
    connection.setNonBlocking();
    connection.allowReusePort();
    connection.bind(Parser::parsePort(port));
    connection.open();
    eventManager.listen(connection);
}

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
    int nev = eventManager.pollEvents();
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
        if (eventManager.isConnectionEvent(i)) {
            acceptConnection();
        } else if (eventManager.isReadableEvent(i)) {
            Socket::fd_t sessionFd = eventManager.getEventSocket(i);
            sessionManager.update(sessionFd);
        }
    }
}

void Server::acceptConnection() {
    try {
        Session *session = connection.accept();
        eventManager.listen(*session);
        sessionManager.add(session);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}