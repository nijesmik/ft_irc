//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

Server::Server(char *port, char *password) :
        password(std::string(password)),
        listenFd(socket(AF_INET, SOCK_STREAM, 0)),
        kqueueFd(kqueue()) {
    if (listenFd < 0) {
        throw std::runtime_error("Error: socket creation failed");
    }
    if (kqueueFd < 0) {
        throw std::runtime_error("Error: kqueue creation failed");
    }

    // bind socket to address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    address.sin_port = htons(Parser::parsePort(port)); // convert to network byte order and set port
    if (bind(listenFd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        throw std::runtime_error("Error: socket binding failed");
    }

    // set socket options
    int optval = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); // allow reuse of address

    // listen for incoming connections
    if (listen(listenFd, SOMAXCONN) < 0) {
        throw std::runtime_error("Error: socket listening failed");
    }

    if (!registerWithKqueue(listenFd, kqueueFd)) {
        throw std::runtime_error("Error: event registration failed");
    }
}

Server::~Server() {
    close(listenFd);
    close(kqueueFd);
    for (std::map<fd_t, User *>::iterator it = session.begin(); it != session.end(); it++) {
        delete it->second;
    }
}

void Server::start() {
    while (run());
}

bool Server::run() {
    // poll for events
    struct kevent eventlist[NEVENTS];
    int nev = kevent(kqueueFd, NULL, 0, eventlist, NEVENTS, NULL);
    switch (nev) {
        case -1: // error
            throw std::runtime_error("Error: event polling failed");
        case 0: // no events
            return true;
    }
    for (int i = 0; i < nev; i++) {
        if (static_cast<int>(eventlist[i].ident) == listenFd) {
            acceptConnection();
        }
    }
    return true;
}

void Server::acceptConnection() {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    fd_t clientFd = accept(listenFd, (struct sockaddr *) &address, &addressLength);
    if (clientFd < 0) {
        std::cerr << "Error: connection accept failed" << std::endl;
        return;
    }
    if (!registerWithKqueue(clientFd, kqueueFd)) {
        std::cerr << "Error: event registration failed" << std::endl;
        return;
    }
    session[clientFd] = new User(clientFd, address);
}