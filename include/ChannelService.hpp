//
// Created by 김세진 on 8/21/24.
//

#ifndef FT_IRC_CHANNELSERVICE_HPP
#define FT_IRC_CHANNELSERVICE_HPP

#endif //FT_IRC_CHANNELSERVICE_HPP

#include <map>
#include "Channel.hpp"
#include "Session.hpp"
#include "Message.hpp"

class ChannelService {
private:
    typedef std::map<std::string, Channel *> Channels;

    Channels channels;

    Channel *createChannel(std::string const &name);
    Channel *findChannel(std::string const &name);
    Channel *getChannel(std::string const &name);

public:
    ChannelService();
    ~ChannelService();

    void join(Session &session, Message const &message);
};