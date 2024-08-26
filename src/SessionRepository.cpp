//
// Created by 김세진 on 8/14/24.
//

#include "SessionRepository.hpp"

SessionRepository *SessionRepository::singleton = NULL;

SessionRepository::SessionRepository(int port) {
    setNonBlocking();
    allowReusePort();
    bind(port);
    open();
}

SessionRepository::~SessionRepository() {
    for (SessionRepository::iterator it = sessions.begin(); it != sessions.end(); it++) {
        delete it->second;
    }
}

SessionRepository *SessionRepository::init(int port) {
    if (singleton == NULL) {
        singleton = new SessionRepository(port);
        return singleton;
    }
    return NULL;
}

SessionRepository *SessionRepository::instance() {
    return singleton;
}

Session *SessionRepository::connect() {
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

Session *SessionRepository::find(Socket::fd_t sessionFd) {
    return sessions[sessionFd];
}

Session *SessionRepository::find(std::string const &nickname) {
    for (SessionRepository::iterator it = sessions.begin(); it != sessions.end(); it++) {
        if (it->second->getNickname() == nickname) {
            return it->second;
        }
    }
    return NULL;
}

void SessionRepository::disconnect(Socket::fd_t sessionFd) {
    if (close(sessionFd) < 0) {
        throw std::runtime_error("Error: session close failed");
    }

    std::map<Socket::fd_t, Session *>::iterator it = sessions.find(sessionFd);
    if (it != sessions.end()) {
        sessions.erase(it);
    } else {
        throw std::runtime_error("Error: session close failed");
    }

    delete it->second;
}
