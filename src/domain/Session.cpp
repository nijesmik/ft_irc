//
// Created by 김세진 on 8/13/24.
//

#include "Session.hpp"
#include "Channel.hpp"

Session::Session(Socket::fd_t fd, std::string const &address) :
        Socket(fd),
        address(address),
        passed(false),
        registered(false) {
    std::cout << "New connection from " << address << std::endl;
}

Session::~Session() {
    std::cout << "Connection closed from " << address << std::endl;
}

bool Session::operator>>(Message &message) {
    std::stringstream sstream;
    if (Socket::operator>>(sstream)) {
        message = Parser::parseMessage(sstream);
        return true;
    }
    return false;
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

Session::Channels Session::getJoinedChannels() const {
    return channels;
}

void Session::leaveChannel(Channel *channel) {
    channels.erase(channel);
}

void Session::joinChannel(Channel *channel) {
    channels.insert(channel);
}