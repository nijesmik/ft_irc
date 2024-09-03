//
// Created by 이석규 on 2024. 8. 31..
//

#include "ChannelService.hpp"

// This command is used to query a list of users who match the provided mask.
// In this server this command only query a list of users in channel.
void ChannelService::who(Session *session, Message const &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    const std::string channelName = message.getParam();
    if (channelName.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "WHO" >> session;
    }

    const Channel *channel = this->findChannel(channelName);
    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }

    NumericReply(RPL_WHOREPLY, channel->getParticipantList()) << session << PUBLIC_CHANNEL_SYMBOL << channelName >> session;
    NumericReply(RPL_ENDOFWHO) << session << channelName >> session;
}
