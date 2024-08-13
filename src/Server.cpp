//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

Server::Server(char *port, char *password) :
        password(std::string(password)),
        serverFd(socket(AF_INET, SOCK_STREAM, 0)) {
    if (serverFd < 0) {
        throw std::runtime_error("Error: socket creation failed");
    }

    // bind socket to address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    address.sin_port = htons(Parser::parsePort(port)); // convert to network byte order and set port
    if (bind(serverFd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        throw std::runtime_error("Error: socket binding failed");
    }

    // set socket options
    int optval = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); // allow reuse of address

    // listen for incoming connections
    if (listen(serverFd, SOMAXCONN) < 0) {
        throw std::runtime_error("Error: socket listening failed");
    }

    if (!eventListener.listen(serverFd)) {
        throw std::runtime_error("Error: event registration failed");
    }
}

Server::~Server() {
    close(serverFd);
    for (std::map<fd_t, User *>::iterator it = session.begin(); it != session.end(); it++) {
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
        struct kevent event = eventListener.getEvent(i);
        fd_t eventFd = static_cast<int>(event.ident);
        if (eventFd == serverFd) {
            acceptConnection();
        }
    }
}

void Server::acceptConnection() {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    fd_t clientFd = accept(serverFd, (struct sockaddr *) &address, &addressLength);
    if (clientFd < 0) {
        std::cerr << "Error: connection accept failed" << std::endl;
        return;
    }
    if (!eventListener.listen(clientFd)) {
        std::cerr << "Error: event registration failed" << std::endl;
        close(clientFd);
        return;
    }
    session[clientFd] = new User(clientFd, address);
}