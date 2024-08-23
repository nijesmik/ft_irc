//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

static bool isValidChannel(const std::string &channel) {
    if (channel.length() < 2 || channel.size() > CHANNELLEN) {
        return false;
    }
    return (channel[0] == '#' || channel[0] == '&');
}

void ChannelService::join(Session &session, const Message &message) {
    if (!session.isRegistered()) {
        return session << NumericReply::get(ERR_NOTREGISTERED);
    }

    std::vector<std::string> channels = message.getSplitedParam(0, ',');
    if (channels.empty()) {
        return session << NumericReply::get(ERR_NEEDMOREPARAMS, "JOIN");
    }

    std::vector<std::string> keys = message.getSplitedParam(1, ',');
    std::vector<std::string>::iterator keyIt = keys.begin();
    for (std::vector<std::string>::iterator channelIt = channels.begin(); channelIt != channels.end(); ++channelIt) {
        if (!isValidChannel(*channelIt)) {
            return session << NumericReply::get(ERR_BADCHANMASK, "JOIN");
        }

        Channel *channel = getChannel(*channelIt);
        channel->join(&session, keyIt != keys.end() ? *keyIt : std::string());

        if (keyIt != keys.end()) {
            ++keyIt;
        }
    }
}
