//
// Created by 김세진 on 8/21/24.
//

#include "Channel.hpp"

Channel::Channel(std::string const &name) : name(name) {}

Channel::~Channel() {}

int Channel::remove(Session *session) {
    session->leaveChannel(name);
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

bool Channel::isOperator(Session *session) const {
    return operators.find(session) != operators.end();
}

bool Channel::isParticipant(Session *session) const {
    return participants.find(session) != participants.end();
}

Session *Channel::getParticipant(std::string const &nickname) const {
    for (Sessions::iterator it = participants.begin(); it != participants.end(); it++) {
        if ((*it)->getNickname() == nickname) {
            return *it;
        }
    }
    return NULL;
}

void Channel::setTopic(std::string const &topicName) {
    // TODO: t 옵션일 시 operator 인지 확인하기

    // TODO: Channel 전체 client에게 message 보낼 때, <바꾼 client> <channel> : <topic>
    this->topic = topicName;
    NumericReply(RPL_TOPIC, this->topic) >> this;
}

void Channel::displayTopic(Session *session) {
    if (topic.empty()) {
        return NumericReply(RPL_NOTOPIC) >> session;
    }
    return NumericReply(RPL_TOPIC, this->topic) >> session;
}

void Channel::setInviteOnly(Channel::mode_t mode) {
    inviteOnly = mode == ADD;
}

void Channel::setTopicRestricted(Channel::mode_t mode) {
    topicRestricted = mode == ADD;
}

int Channel::setKey(Channel::mode_t mode, std::string const &key) {
    if (key.empty()) {
        return 0;
    }
    if (mode == ADD) {
        this->key = key;
        return 1;
    }
    this->key.clear();
    return 0;
}

int Channel::setLimit(Channel::mode_t mode, std::string const &limit, Session *session) {
    if (mode == REMOVE) {
        this->limit = 0;
        return 0;
    }
    if (limit.empty()) {
        return 0;
    }
    try {
        this->limit = Parser::parseLimit(limit);
    } catch (std::exception &e) {
        // ERR_INVALIDMODEPARAM
    }
    return 1;
}

int Channel::setOperator(Channel::mode_t mode, const std::string &nickname, Session *session) {
    if (nickname.empty()) {
        // the contents of the list MUST be sent to the user
        return 0;
    }
    Session *participant = getParticipant(nickname);
    if (!participant) {
        NumericReply(ERR_USERNOTINCHANNEL) << session << nickname << name >> session;
        return 1;
    }
    if (mode == ADD) {
        operators.insert(participant);
    } else {
        operators.erase(participant);
    }
    return 1;
}