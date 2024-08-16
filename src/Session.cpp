//
// Created by 김세진 on 8/13/24.
//

#include "Session.hpp"

Session::Session(Socket::fd_t fd) :
        Socket(fd),
        registered(false) {}

Session::~Session() {}

Session &Session::read() {
    Socket::read();
    return *this;
}

void Session::operator>>(Message &message) {
    std::stringstream sstream;
    try {
        Socket::operator>>(sstream);
        message = Parser::parseMessage(sstream);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

bool Session::isRegistered() const {
    return this->registered;
}

void Session::signUp() {
    this->registered = true;
}