//
// Created by 김세진 on 8/24/24.
//

#include "Channel.hpp"

std::string RPL_MODE(Session *session, std::string const &channelName, std::string const &modestring,
                     std::string const &param);

int Channel::setMode(Channel::mode_t mode, char modeChar, const std::string &param, Session *session) {
    switch (modeChar) {
        case 'i':
            return (setInviteOnly(session, mode), 0);
        case 't':
            return (setTopicRestricted(session, mode), 0);
        case 'k':
            return setKey(session, mode, param);
        case 'l':
            return setLimit(session, mode, param);
        case 'o':
            return setOperator(session, mode, param);
        default:
            return 0;
    }
}

void Channel::displayMode(Session *session) const {
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
    return NumericReply(RPL_CHANNELMODEIS) << session << info.str() >> session;
}

void Channel::setInviteOnly(Session *session, Channel::mode_t mode) {
    if (mode == ADD && !inviteOnly) {
        inviteOnly = true;
        broadcast(RPL_MODE(session, name, "+i", ""));
    }
    if (mode == REMOVE && inviteOnly) {
        inviteOnly = false;
        broadcast(RPL_MODE(session, name, "-i", ""));
    }
}

void Channel::setTopicRestricted(Session *session, Channel::mode_t mode) {
    if (mode == ADD && !topicRestricted) {
        topicRestricted = true;
        broadcast(RPL_MODE(session, name, "+t", ""));
    }
    if (mode == REMOVE && topicRestricted) {
        topicRestricted = false;
        broadcast(RPL_MODE(session, name, "-t", ""));
    }
}

int Channel::setKey(Session *session, Channel::mode_t mode, std::string const &key) {
    if (key.empty()) {
        return 0;
    }
    if (mode == ADD) {
        this->channelKey = key;
        broadcast(RPL_MODE(session, name, "+k", key));
        return 1;
    }
    if (!this->channelKey.empty()) {
        this->channelKey.clear();
        broadcast(RPL_MODE(session, name, "-k", ""));
    }
    return 0;
}

int Channel::setLimit(Session *session, Channel::mode_t mode, std::string const &limit) {
    if (mode == REMOVE) {
        this->limit = 0;
        broadcast(RPL_MODE(session, name, "-l", ""));
        return 0;
    }
    if (limit.empty()) {
        return 0;
    }
    try {
        this->limit = Parser::parseLimit(limit);
        broadcast(RPL_MODE(session, name, "+l", limit));
    } catch (std::exception &e) {
        NumericReply(ERR_INVALIDMODEPARAM) << session << name << "+l" << limit >> session;
    }
    return 1;
}

int Channel::setOperator(Session *session, Channel::mode_t mode, const std::string &nickname) {
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
    if (mode == ADD && !isOperator(participant)) {
        operators.insert(participant);
        broadcast(RPL_MODE(session, name, "+o", nickname));
    }
    if (mode == REMOVE && isOperator(participant)) { ;
        operators.erase(participant);
        broadcast(RPL_MODE(session, name, "-o", nickname));
    }
    return 1;
}

std::string RPL_MODE(Session *session, std::string const &channelName, std::string const &modestring,
                     std::string const &param) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << session->getAddress() << DELIMITER
       << "MODE" << DELIMITER
       << channelName << DELIMITER
       << modestring;
    if (!param.empty()) {
        ss << DELIMITER << param;
    }
    ss << CRLF;
    return ss.str();
}
