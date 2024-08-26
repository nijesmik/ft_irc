//
// Created by 김세진 on 8/21/24.
//

#ifndef FT_IRC_CHANNELSERVICE_HPP
#define FT_IRC_CHANNELSERVICE_HPP

#include <map>
#include "Channel.hpp"
#include "Session.hpp"
#include "Message.hpp"
#include "NumericReply.hpp"

#define CHANNELLEN 50

class ChannelService {
private:
    typedef std::map<std::string, Channel *> Channels;

    Channels channels;

    Channel *createChannel(std::string const &name);
    Channel *findChannel(std::string const &name);
    Channel *getChannel(std::string const &name);
    void deleteChannel(std::string const &name);

    void part(Session &session, std::string const &channelName, std::string const &reason);

public:
    ChannelService();
    ~ChannelService();

    void join(Session &session, Message const &message);
    void part(Session &session, Message const &message);
    void kick(Session *session, Message const &message);
    void topic(Session &session, Message const &message);
    void mode(Session *session, Message const &message);
};

#endif //FT_IRC_CHANNELSERVICE_HPP
