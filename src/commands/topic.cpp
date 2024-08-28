//
// Created by 이석규 on 2024. 8. 22..
//

#include "ChannelService.hpp"

void ChannelService::topic(Session *session, const Message &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::string const &channelName = message.getParam();
    if (channelName.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "TOPIC" >> session;
    }

    std::string topicName = message.joinParams(1, ':');
    Channel *channel = findChannel(channelName);

    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }
    if (!channel->isParticipant(session)) {
        return NumericReply(ERR_NOTONCHANNEL) << session << channelName >> session;
    }
    channel->topic(session, topicName);
}

