//
// Created by 김세진 on 8/21/24.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include <set>
#include "Session.hpp"
#include "NumericReply.hpp"

class Channel {
public:
    typedef enum mode_e {
        ADD, // +
        REMOVE, // -
    } mode_t;

    Channel(std::string const &name);
    ~Channel();

    bool isInviteOnly() const;
    bool isOperator(Session *session) const;
    bool isParticipant(Session *session) const;
    void addParticipant(Session *session);
    Session *getParticipant(std::string const &nickname) const;
    std::string getOperatorList() const;

    void operator<<(std::string const &message);

    void broadcast(std::string const &message);
    void join(Session *session, const std::string &key);
    size_t remove(Session *session);

    // src/Channel/mode.cpp
    int mode(Channel::mode_t mode, char modeChar, const std::string &param, Session *session);
    std::string getModeInfo() const;

    // src/Channel/topic.cpp
    void setTopic(Session *session, std::string const &topicName);
    void displayTopic(Session *session);

private:
    typedef std::set<Session *> Sessions;

    const std::string name;
    Sessions operators;
    std::string channelTopic;
    Sessions participants;
    bool inviteOnly;
    bool topicRestricted;
    std::string channelKey;
    size_t limit;

    // Channel/mode.cpp
    void setInviteOnly(Session *session, Channel::mode_t mode);
    void setTopicRestricted(Session *session, Channel::mode_t mode);
    int setKey(Session *session, Channel::mode_t mode, std::string const &key);
    int setLimit(Session *session, Channel::mode_t mode, std::string const &limit);
    int setOperator(Session *session, Channel::mode_t mode, std::string const &nickname);
};

#endif //FT_IRC_CHANNEL_HPP
