//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

ChannelService::ChannelService(SessionRepository *repository) : sessionRepository(repository) {}

ChannelService::~ChannelService() {
    for (Channels::iterator it = channels.begin(); it != channels.end(); it++) {
        delete it->second;
    }
}

Channel *ChannelService::createChannel(const std::string &name, Session *session, const std::string &key) {
    Channel *channel = new Channel(name, session, key);
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

void ChannelService::deleteChannel(const std::string &name) {
    Channels::iterator it = channels.find(name);
    if (it != channels.end()) {
        Channel *channel = it->second;
        channels.erase(it);
        delete channel;
    }
}