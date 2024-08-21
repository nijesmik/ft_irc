//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

ChannelService::ChannelService() {}

ChannelService::~ChannelService() {
    for (Channels::iterator it = channels.begin(); it != channels.end(); it++) {
        delete it->second;
    }
}

Channel *ChannelService::createChannel(const std::string &name) {
    Channel *channel = new Channel(name);
    channels[name] = channel;
    return channel;
}

Channel *ChannelService::findChannel(const std::string &name) {
    Channels::iterator it = channels.find(name);
    if (it == channels.end()) {
        return NULL;
    }
    return it->second;
}

Channel *ChannelService::getChannel(const std::string &name) {
    Channel *channel = findChannel(name);
    if (channel) {
        return channel;
    }
    return createChannel(name);
}