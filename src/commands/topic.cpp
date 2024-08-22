//
// Created by 이석규 on 2024. 8. 22..
//

#include "ChannelService.hpp"

void ChannelService::topic(Session &session, const Message &message) {
    if (!session.isRegistered()) {
        return session << NumericReply::get(ERR_NOTREGISTERED);
    }
    if (message.getParam().empty()) {
        return session << NumericReply::get(ERR_NEEDMOREPARAMS);
    }

    std::string channelName = message.getParam();
    std::string topicName = message.getParamsAll(1, ':');
    Channel *channel = findChannel(channelName);

    //
}