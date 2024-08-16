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

Session *SessionManager::get(Socket::fd_t sessionFd) {
    return sessions[sessionFd];
}