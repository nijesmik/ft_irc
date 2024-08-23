//
// Created by 이석규 on 2024. 8. 22..
//

#include "ChannelService.hpp"

void ChannelService::topic(Session &session, const Message &message) {
    if (!session.isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }
    if (message.getParam().empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "TOPIC" >> session;
    }

    std::string channelName = message.getParam();
    std::string topicName = message.getParamsAll(1, ':');
    bool newTopic = message.getParamSize() > 1;
    Channel *channel = findChannel(channelName);

    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }
    if (channel->isParticipant(&session)) {
        return NumericReply(ERR_NOTONCHANNEL) << session << channelName >> session;
    }

    if (newTopic) {
        channel->setTopic(topicName);
    }
    return channel->displayTopic(&session);
}
