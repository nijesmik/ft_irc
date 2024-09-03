//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

std::string RPL_PART(Session const &session, const std::string &channelName, const std::string &reason);

typedef std::vector<std::string>::const_iterator iterator;

void ChannelService::part(Session *session, const Message &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::vector<std::string> channels = message.splitParam(0, ',');
    std::string const &reason = message.getParam(1);
    if (channels.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "PART" >> session;
    }

    for (iterator it = channels.begin(); it != channels.end(); it++) {
        part(session, *it, reason);
    }
}

void ChannelService::part(Session *session, std::string const &channelName, std::string const &reason) {
    Channel *channel = findChannel(channelName);
    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }

    if (!channel->isParticipant(session)) {
        return NumericReply(ERR_NOTONCHANNEL) << session << channelName >> session;
    }

    int remain = channel->removeParticipant(session);
    std::string const &reply = RPL_PART(*session, channelName, reason);
    *session << reply;
    if (remain) {
        return channel->broadcast(reply);
    }
    deleteChannel(channelName);
}

std::string RPL_PART(Session const &session, const std::string &channelName, const std::string &reason) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << session.getAddress() << DELIMITER
       << "PART" << DELIMITER
       << channelName;
    if (!reason.empty()) {
        ss << DELIMITER << MESSAGE_PREFIX << reason;
    }
    ss << CRLF;
    return ss.str();
}