//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

Server::Server(char *port, char *password) :
        password(std::string(password)),
        eventListener(connection) {
    connection.setNonBlocking();
    connection.allowReusePort();
    connection.bind(Parser::parsePort(port));
    connection.open();
    eventListener.listen(connection);
}

Server::~Server() {
    for (std::map<Socket::fd_t, Client *>::iterator it = session.begin(); it != session.end(); it++) {
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
    int nev = eventListener.pollEvents();
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
        if (eventListener.isConnectionEvent(i)) {
            acceptConnection();
        } else if (eventListener.canReadEvent(i)) {
            readEventSocket(i);
        }
    }
}

void Server::readEventSocket(int index) {
    try {
        Socket::fd_t client = eventListener.getEventSocket(index);
        session[client]->read();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Server::acceptConnection() {
    try {
        Client *client = connection.accept();
        eventListener.listen(*client);
        session[client->getFd()] = client;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}