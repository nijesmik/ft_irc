//
// Created by 김세진 on 8/29/24.
//

#include "BotService.hpp"
#include "NumericReply.hpp"

BotService::BotService() {
    std::srand(std::time(0));
}

BotService::~BotService() {}

std::set<int> getRandomNumbers(unsigned int count, unsigned int max) {
    std::set<int> numbers;
    while (numbers.size() < count) {
        numbers.insert(std::rand() % max + 1);
    }
    return numbers;
}

std::string RPL_BOT(Session const &session, std::set<int> const &lottery) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << session.getAddress() << DELIMITER
       << "BOT" << DELIMITER
       << MESSAGE_PREFIX << "Lottery numbers are ";
    for (std::set<int>::iterator it = lottery.begin(); it != lottery.end(); it++) {
        if (it != lottery.begin()) {
            ss << ", ";
        }
        ss << *it;
    }
    ss << CRLF;
    return ss.str();
}

void BotService::makeLottery(Session *session) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }
    std::set<int> const &lottery = getRandomNumbers(6, 45);
    *session << RPL_BOT(*session, lottery);
}

