//
// Created by 김세진 on 8/21/24.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include <set>
#include "Session.hpp"

class Channel {
private:
    typedef std::set<Session *> Sessions;

    const std::string name;
    Sessions operators;
    Sessions participants;

public:
    Channel(std::string const &name);
    ~Channel();

    bool isOperator(Session *session) const;
    bool isParticipant(Session *session) const;
    Session *getParticipant(std::string const &nickname) const;

    void operator<<(std::string const &message);

    void broadcast(std::string const &message);
    void join(Session *session);
    int remove(Session *session);
};

#endif //FT_IRC_CHANNEL_HPP
