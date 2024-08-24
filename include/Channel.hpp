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
    std::string topic;
    Sessions participants;
    bool inviteOnly;
    bool topicRestricted;
    std::string key;
    int limit;

public:
    typedef enum mode_e {
        ADD, // +
        REMOVE, // -
    } mode_t;

    Channel(std::string const &name);
    ~Channel();

    bool isOperator(Session *session) const;
    bool isParticipant(Session *session) const;
    Session *getParticipant(std::string const &nickname) const;

    void operator<<(std::string const &message);

    void broadcast(std::string const &message);
    void join(Session *session, const std::string &key);
    int remove(Session *session);
    void setTopic(std::string const &topicName);
    void displayTopic(Session *session);

    int mode(Channel::mode_t mode, char modeChar, const std::string &param, Session *session);
    std::string getModeInfo() const;
    void setInviteOnly(Channel::mode_t mode);
    void setTopicRestricted(Channel::mode_t mode);
    int setKey(Channel::mode_t mode, std::string const &key);
    int setLimit(Channel::mode_t mode, std::string const &limit, Session *session);
    int setOperator(Channel::mode_t mode, std::string const &nickname, Session *session);
};

#endif //FT_IRC_CHANNEL_HPP
