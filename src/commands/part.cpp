//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

#define RPL_CHANNELPART(clientAddress, channelName, reason) (":" + clientAddress + " PART " + channelName + " " + reason + CRLF)

typedef std::vector<std::string>::const_iterator iterator;

void ChannelService::part(Session &session, const Message &message) {
    if (!session.isRegistered()) {
        return session << NumericReply::get(ERR_NOTREGISTERED);
    }

    std::vector<std::string> channels = message.getSplitedParam(0, ',');
    std::string reason = message.getParamsAll(1);
    if (channels.empty()) {
        return session << NumericReply::get(ERR_NEEDMOREPARAMS, "PART");
    }

    for (iterator it = channels.begin(); it != channels.end(); it++) {
        part(session, *it, reason);
    }
}

void ChannelService::part(Session &session, std::string const &channelName, std::string const &reason) {
    if (!findChannel(channelName)) {
        return session << NumericReply::channelReply(ERR_NOSUCHCHANNEL, session.getNickname(), channelName);
    }

    Channel *channel = session.findJoinedChannel(channelName);
    if (!channel) {
        return session << NumericReply::channelReply(ERR_NOTONCHANNEL, session.getNickname(), channelName);
    }

    session.leaveChannel(channelName);
    channel->part(&session);
    std::string reply = RPL_CHANNELPART(session.getAddress(), channelName, reason);
    channel->broadcast(reply);
    session << reply;
}