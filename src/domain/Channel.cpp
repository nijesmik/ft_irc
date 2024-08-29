//
// Created by 김세진 on 8/21/24.
//

#include "Channel.hpp"

Channel::Channel(std::string const &name) : name(name) {}

Channel::~Channel() {}

size_t Channel::removeParticipant(Session *session) {
    session->leaveChannel(this);
    operators.erase(session);
    participants.erase(session);
    return participants.size();
}

void Channel::operator<<(const std::string &message) {
    broadcast(message);
}

void Channel::broadcast(std::string const &message) {
    for (Sessions::iterator it = participants.begin(); it != participants.end(); it++) {
        (*it)->operator<<(message);
    }
}

std::string const &Channel::getName() const {
    return name;
}

bool Channel::isInviteOnly() const {
    return inviteOnly;
}

bool Channel::isOperator(Session *session) const {
    return operators.find(session) != operators.end();
}

void Channel::setOperator(Session *session) {
    operators.insert(session);
}

bool Channel::isParticipant(Session *session) const {
    return participants.find(session) != participants.end();
}

void Channel::addParticipant(Session *session) {
    session->joinChannel(this);
    participants.insert(session);
}

Session *Channel::getParticipant(std::string const &nickname) const {
    for (Sessions::iterator it = participants.begin(); it != participants.end(); it++) {
        if ((*it)->getNickname() == nickname) {
            return *it;
        }
    }
    return NULL;
}

std::string Channel::getParticipantList() const {
    std::stringstream list;
    for (Sessions::const_iterator it = participants.begin(); it != participants.end(); it++) {
        if (isOperator(*it)) {
            list << OPERATOR_PREFIX;
        }
        list << (*it)->getNickname() << DELIMITER;
    }
    return list.str();
}

std::string Channel::getOperatorList() const {
    std::stringstream list;
    for (Sessions::const_iterator it = operators.begin(); it != operators.end(); it++) {
        list << OPERATOR_PREFIX << (*it)->getNickname() << DELIMITER;
    }
    return list.str();
}
