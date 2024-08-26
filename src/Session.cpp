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

bool Session::operator==(Session const &other) {
    return this->getFd() == other.getFd();
}

bool Session::isPassed() const {
    return this->passed;
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

void Session::updateUser(std::string const &username, std::string const &hostname, std::string const &servername,
                         std::string const &realname) {
    this->username = username;
    this->hostname = hostname;
    this->servername = servername;
    this->realname = realname;
}

Channel *Session::findJoinedChannel(std::string const &name) const {
    Channels::const_iterator it = channels.find(name);
    if (it == channels.end()) {
        return NULL;
    }
    return it->second;
}

std::vector<Channel *> Session::getAffiliatedChannel() const {
    std::vector<Channel *> affiliatedChannels;
    Channels::const_iterator it;
    for (it = channels.begin(); it != channels.end(); it++) {
        affiliatedChannels.push_back(it->second);
    }

    return affiliatedChannels;
}

void Session::leaveChannel(std::string const &name) {
    Channels::iterator it = channels.find(name);
    channels.erase(it);
}