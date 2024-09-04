//
// Created by 김세진 on 8/21/24.
//

#ifndef FT_IRC_CHANNELSERVICE_HPP
#define FT_IRC_CHANNELSERVICE_HPP

#include <map>
#include "Channel.hpp"
#include "SessionRepository.hpp"
#include "Session.hpp"
#include "Message.hpp"
#include "NumericReply.hpp"

#define CHANNELLEN 50

class ChannelService {
private:
    typedef std::map<std::string, Channel *> Channels;

    Channels channels;
    SessionRepository *sessionRepository;

    Channel *createChannel(std::string const &name, Session *session, const std::string &key);
    Channel *findChannel(std::string const &name);
    void deleteChannel(std::string const &name);

    void part(Session *session, std::string const &channelName, std::string const &reason);
    void privmsgToChannel(Session *session, std::string const &target, std::string const &text);
    void privmsgToUser(Session *session, std::string const &target, std::string const &text);

public:
    ChannelService(SessionRepository *sessionRepository);
    ~ChannelService();

    void join(Session *session, Message const &message);
    void part(Session *session, Message const &message);
    void kick(Session *session, Message const &message);
    void topic(Session *session, Message const &message);
    void mode(Session *session, Message const &message);
    void quit(Session *session, std::string const &reason);
    void invite(Session *session, Message const &message);
    void privmsg(Session *session, Message const &message);
    void who(Session *session, Message const &message);
};

#endif //FT_IRC_CHANNELSERVICE_HPP
