//
// Created by 김세진 on 8/13/24.
//

#include "Session.hpp"

Session::Session(Socket::fd_t fd) :
        Socket(fd),
        passed(false),
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

void Session::setPassed() {
    this->passed = true;
}

void Session::setRegistered() {
    this->registered = true;
}

std::string const &Session::getNickname() const {
    return this->nickname;
}

std::string const &Session::getUsername() const {
    return this->username;
}

std::string const &Session::getServername() const {
    return this->servername;
}

std::string Session::getAddress() const {
    return nickname + "!" + username + "@" + hostname;
}

void Session::updateNickname(std::string const &nickname) {
    this->nickname = nickname;
}
