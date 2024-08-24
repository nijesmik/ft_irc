//
// Created by 김세진 on 8/23/24.
//

#include "ChannelService.hpp"

typedef std::string::const_iterator str_iter;

bool isValidModeString(std::string const &modestring, Session *session);

void ChannelService::mode(Session *session, const Message &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::string const &channelName = message.getParam(0);
    Channel *channel = findChannel(channelName);
    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }

    std::string const &modestring = message.getParam(1);
    if (modestring.empty()) {
        return NumericReply(RPL_CHANNELMODEIS) << session << channel->getModeInfo() >> session;
    }
    if (!channel->isOperator(session)) {
        return NumericReply(ERR_CHANOPRIVSNEEDED) << session << channelName >> session;
    }
    if (!isValidModeString(modestring, session)) {
        return;
    }

    Channel::mode_t mode;
    int index = 2;
    for (str_iter it = modestring.begin(); it != modestring.end(); it++) {
        switch (*it) {
            case '+':
                mode = Channel::ADD;
                break;
            case '-':
                mode = Channel::REMOVE;
                break;
            default:
                index += channel->mode(mode, *it, message.getParam(index), session);
        }
    }
}

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

bool isPlusMinus(char c) {
    return c == '+' || c == '-';
}

bool isModeChar(char c) {
    return c == 'i' || c == 't' || c == 'k' || c == 'l' || c == 'o';
}

bool isValidModeString(std::string const &modestring, Session *session) {
    str_iter it = modestring.begin();
    if (!isPlusMinus(*it)) {
        NumericReply(ERR_NEEDMOREPARAMS) << session << "+ or -" >> session;
        return false;
    }
    for (; it != modestring.end(); it++) {
        if (!isPlusMinus(*it) && !isModeChar(*it)) {
            NumericReply(ERR_UNKNOWNMODE) << session << std::string(1, *it) >> session;
            return false;
        }
    }
    return true;
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