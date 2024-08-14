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

Server::~Server() {
    for (std::map<Socket::fd_t, Session *>::iterator it = sessions.begin(); it != sessions.end(); it++) {
        delete it->second;
    }
}

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
            readEventSocket(i);
        }
    }
}

void Server::readEventSocket(int index) {
    std::stringstream sstream;

    try {
        Socket::fd_t sessionFd = eventManager.getEventSocket(index);
        sessions[sessionFd]->read() >> sstream;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Server::acceptConnection() {
    try {
        Session *session = connection.accept();
        eventManager.listen(*session);
        sessions[session->getFd()] = session;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}