//
// Created by 김세진 on 8/21/24.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include <vector>
#include "Session.hpp"

class Channel {
private:
    const std::string name;
    std::vector<Session> participants;

public:
    Channel(std::string const &name);
    ~Channel();

    void join(Session const &session);
};

#endif //FT_IRC_CHANNEL_HPP
