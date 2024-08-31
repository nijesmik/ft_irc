//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

typedef std::vector<std::string>::const_iterator str_iter;
bool isValidChannel(const std::string &channel);

void ChannelService::join(Session *session, const Message &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }
    std::vector<std::string> channelNames = message.splitParam(0, ',');
    if (channelNames.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "JOIN" >> session;
    }

    std::vector<std::string> keys = message.splitParam(1, ',');
    str_iter keyIt = keys.begin();
    str_iter channelIt = channelNames.begin();
    for (size_t i = 0; i < channelNames.size(); i++) {
        std::string const &channelName = *(channelIt + i);
        std::string const &key = i < keys.size() ? *(keyIt + i) : std::string();
        if (!isValidChannel(channelName)) {
            return NumericReply(ERR_BADCHANMASK) << channelName >> session;
        }

        Channel *channel = findChannel(channelName);
        if (!channel) {
            channel = createChannel(channelName, session, key);
        }
        channel->join(session, key);
    }
}

bool isValidChannel(const std::string &channel) {
    if (channel.length() < 2 || channel.size() > CHANNELLEN) {
        return false;
    }
    return (channel[0] == '#' || channel[0] == '&');
}
