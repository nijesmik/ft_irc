//
// Created by 김세진 on 8/29/24.
//

#ifndef FT_IRC_BOTSERVICE_HPP
#define FT_IRC_BOTSERVICE_HPP

#include "Session.hpp"
#include <set>
#include <ctime> // std::time
#include <cstdlib> // std::rand
#include <sstream>

class BotService {
public:
    BotService();
    ~BotService();

    void makeLottery(Session *session);
};

#endif //FT_IRC_BOTSERVICE_HPP
