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
    Sessions participants;

public:
    Channel(std::string const &name);
    ~Channel();

    void broadcast(std::string const &message);
    void join(Session *session);
    void part(Session *session);
};

#endif //FT_IRC_CHANNEL_HPP
