//
// Created by 김세진 on 8/14/24.
//

#include "SessionRepository.hpp"

SessionRepository *SessionRepository::singleton = NULL;

SessionRepository::SessionRepository() {}

SessionRepository::~SessionRepository() {
    for (SessionRepository::iterator it = sessions.begin(); it != sessions.end(); it++) {
        delete it->second;
    }
}

SessionRepository *SessionRepository::init() {
    if (singleton == NULL) {
        singleton = new SessionRepository();
        return singleton;
    }
    return NULL;
}

SessionRepository *SessionRepository::instance() {
    return singleton;
}

void SessionRepository::destroy() {
    delete singleton;
    singleton = NULL;
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

void SessionRepository::add(Session *session) {
    sessions[session->getFd()] = session;
}

void SessionRepository::remove(Socket::fd_t sessionFd) {
    SessionRepository::iterator it = sessions.find(sessionFd);
    if (it != sessions.end()) {
        Session *session = it->second;
        sessions.erase(it);
        delete session;
    }
}
