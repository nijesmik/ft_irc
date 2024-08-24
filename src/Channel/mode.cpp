//
// Created by 김세진 on 8/24/24.
//

#include "Channel.hpp"

int Channel::mode(Channel::mode_t mode, char modeChar, const std::string &param, Session *session) {
    switch (modeChar) {
        case 'i':
            return (setInviteOnly(mode), 0);
        case 't':
            return (setTopicRestricted(mode), 0);
        case 'k':
            return setKey(mode, param);
        case 'l':
            return setLimit(mode, param, session);
        case 'o':
            return setOperator(mode, param, session);
        default:
            return 0;
    }
}

std::string Channel::getModeInfo() const {
    std::stringstream info;
    info << name << DELIMITER << "+";
    if (inviteOnly) {
        info << "i";
    }
    if (topicRestricted) {
        info << "t";
    }
    if (limit) {
        info << "l" << DELIMITER << limit;
    }
    return info.str();
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
        NumericReply(ERR_INVALIDMODEPARAM) << session << name << "+l" << limit >> session;
    }
    return 1;
}

int Channel::setOperator(Channel::mode_t mode, const std::string &nickname, Session *session) {
    if (nickname.empty()) {
        NumericReply(RPL_NAMREPLY, getOperatorList()) << session << PUBLIC_CHANNEL_SYMBOL << name >> session;
        NumericReply(RPL_ENDOFNAMES) << session << name >> session;
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
