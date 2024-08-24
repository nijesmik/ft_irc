//
// Created by 김세진 on 8/21/24.
//

#include "Channel.hpp"

Channel::Channel(std::string const &name) : name(name) {}

Channel::~Channel() {}

int Channel::remove(Session &session) {
    session.leaveChannel(name);
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

bool Channel::isOperator(Session &session) const {
    return operators.find(session) != operators.end();
}

bool Channel::isParticipant(Session &session) const {
    return participants.find(session) != participants.end();
}

bool Channel::hasParticipant(const std::string &nickname) const {
    for (Sessions::const_iterator it = participants.begin(); it != participants.end(); ++it) {
        if (it->getNickname() == nickname) {
            return true;
        }
    }
    return false;
}

// 무조건 있다고 가정 (has -> get)
Session &Channel::getParticipant(std::string const &nickname) {
    Sessions::const_iterator it;
    for (it = participants.begin(); it != participants.end(); ++it) {
        if (it->getNickname() == nickname) {
            return it;
        }
    }
}

void Channel::setTopic(std::string const &topicName) {
    // TODO: t 옵션일 시 operator 인지 확인하기

    // TODO: Channel 전체 client에게 message 보낼 때, <바꾼 client> <channel> : <topic>
    this->topic = topicName;
    NumericReply(RPL_TOPIC, this->topic) >> this;
}

void Channel::displayTopic(Session &session) const {
    if (topic.empty()) {
        return NumericReply(RPL_NOTOPIC) >> session;
    }
    return NumericReply(RPL_TOPIC, this->topic) >> session;
}
