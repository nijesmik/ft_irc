//
// Created by 김세진 on 8/27/24.
//

#include "ChannelService.hpp"

typedef std::vector<std::string>::const_iterator str_iter;

std::string RPL_PRIVMSG(Session const &session, std::string const &target, std::string const &text);

void ChannelService::privmsg(Session *session, const Message &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }
    if (message.getParamSize() < 2) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "PRIVMSG" >> session;
    }

    std::vector<std::string> const &targets = message.splitParam(0, ',');
    std::string const &text = message.getParam(1);
    for (str_iter it = targets.begin(); it != targets.end(); it++) {
        if (Channel::hasChannelMask(*it)) {
            privmsgToChannel(session, *it, text);
        } else {
            privmsgToUser(session, *it, text);
        }
    }
}

void ChannelService::privmsgToChannel(Session *session, std::string const &target, std::string const &text) {
    Channel *channel = findChannel(target);
    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << target >> session;
    }
    if (!channel->isParticipant(session)) {
        return NumericReply(ERR_NOTONCHANNEL) << session << target >> session;
    }
    channel->broadcast(RPL_PRIVMSG(*session, target, text), session);
}

void ChannelService::privmsgToUser(Session *session, std::string const &target, std::string const &text) {
    Session *user = sessionRepository->find(target);
    if (!user) {
        return NumericReply(ERR_NOSUCHNICK) << session << target >> session;
    }
    *user << RPL_PRIVMSG(*session, target, text);
}

std::string RPL_PRIVMSG(Session const &session, std::string const &target, std::string const &text) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << session.getAddress() << DELIMITER
       << "PRIVMSG" << DELIMITER
       << target << DELIMITER
       << MESSAGE_PREFIX << text << CRLF;
    return ss.str();
}