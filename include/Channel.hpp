//
// Created by 김세진 on 8/21/24.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include <set>
#include "Session.hpp"
#include "NumericReply.hpp"

class Channel {
private:
    typedef std::set<Session *> Sessions;

    const std::string name;
    Sessions operators;
    Sessions participants;
    std::string topic;
    bool topicRestricted;
    bool inviteOnly;
    std::string key;
    int limit;

public:
    Channel(std::string const &name);
    ~Channel();

    bool isOperator(Session &session) const;
    bool isParticipant(Session &session) const;
    bool hasParticipant(const std::string &nickname) const;
    Session *getParticipant(std::string const &nickname);

    void operator<<(std::string const &message);

    void broadcast(std::string const &message);
    void join(Session &session, const std::string &key);
    size_t remove(Session &session);
    void setTopic(std::string const &topicName);
    void displayTopic(Session &session) const;
};

#endif //FT_IRC_CHANNEL_HPP
