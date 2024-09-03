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
    if (channelName.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "MODE" >> session;
    }

    if (!Channel::hasChannelMask(channelName)) {
        return; // user mode인 경우 무시
    }

    Channel *channel = findChannel(channelName);
    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }

    std::string const &modestring = message.getParam(1);
    if (modestring.empty()) {
        return channel->displayMode(session);
    }
    if (modestring == "b") {
        return;
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
                index += channel->setMode(mode, *it, message.getParam(index), session);
        }
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
