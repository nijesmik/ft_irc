//
// Created by 김세진 on 8/14/24.
//

#include "SessionManager.hpp"

SessionManager::SessionManager(int port) {
    setNonBlocking();
    allowReusePort();
    bind(port);
    open();
}

SessionManager::~SessionManager() {
    for (SessionManager::iterator it = sessions.begin(); it != sessions.end(); it++) {
        delete it->second;
    }
}

Session *SessionManager::accept() {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);

    // accept new connection
    fd_t socket = ::accept(fd, (struct sockaddr *) &address, &addressLength);
    if (socket < 0) {
        throw std::runtime_error("Error: connection accept failed");
    }

    // display new connection
    std::string ip = std::string(inet_ntoa(address.sin_addr));
    int port = ntohs(address.sin_port);
    std::cout << "New connection from " << ip << ":" << port << std::endl;

    // register new session
    Session *session = new Session(socket);
    sessions[socket] = session;
    return session;
}

Session *SessionManager::get(Socket::fd_t sessionFd) {
    return sessions[sessionFd];
}