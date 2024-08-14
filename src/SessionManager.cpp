//
// Created by 김세진 on 8/14/24.
//

#include "SessionManager.hpp"

SessionManager::SessionManager() {}

SessionManager::~SessionManager() {
    for (SessionManager::iterator it = sessions.begin(); it != sessions.end(); it++) {
        delete it->second;
    }
}

void SessionManager::add(Session *session) {
    sessions[session->getFd()] = session;
}

void SessionManager::update(Socket::fd_t sessionFd) {
    Session *session = sessions[sessionFd];
    std::stringstream sstream;
    try {
        session->read() >> sstream;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}