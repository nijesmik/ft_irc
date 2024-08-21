//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

void ChannelService::join(Session &session, const Message &message) {
    std::string channelName = message.getParam();
    Channel *channel = getChannel(channelName);
    channel->join(session);
}