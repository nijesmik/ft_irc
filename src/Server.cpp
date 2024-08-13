//
// Created by 김세진 on 8/12/24.
//

#include "Server.hpp"

Server::Server(char *port, char *password) :
        password(std::string(password)),
        serverSocket(socket(AF_INET, SOCK_STREAM, 0)),
        eventListener(serverSocket) {
    if (serverSocket < 0) {
        throw std::runtime_error("Error: socket creation failed");
    }

    // set socket non-blocking
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        throw std::runtime_error("Error: socket non-blocking failed");
    }

    // set socket options
    int optval = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); // allow reuse of address

    // bind socket to address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    address.sin_port = htons(Parser::parsePort(port)); // convert to network byte order and set port
    if (bind(serverSocket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        throw std::runtime_error("Error: socket binding failed");
    }

    // listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) < 0) {
        throw std::runtime_error("Error: socket listening failed");
    }

    if (!eventListener.listen(serverSocket)) {
        throw std::runtime_error("Error: event registration failed");
    }
}

Server::~Server() {
    close(serverSocket);
    for (std::map<fd_t, Client *>::iterator it = session.begin(); it != session.end(); it++) {
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
        fd_t client = eventListener.getEventSocket(index);
        session[client]->read();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Server::acceptConnection() {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    fd_t clientFd = accept(serverSocket, (struct sockaddr *) &address, &addressLength);
    if (clientFd < 0) {
        std::cerr << "Error: connection accept failed" << std::endl;
        return;
    }
    if (!eventListener.listen(clientFd)) {
        std::cerr << "Error: event registration failed" << std::endl;
        close(clientFd);
        return;
    }
    session[clientFd] = new Client(clientFd, address);
}