//
// Created by 김세진 on 8/18/24.
//

#include "ConnectionService.hpp"

std::string RPL_NICK(std::string const &oldNick, std::string const &newNick);

typedef std::string::const_iterator str_iter;
bool isCharacterValid(char c);
bool isNicknameValid(std::string const &nickname);

void ConnectionService::nick(Session &session, const Message &message) {
    if (!session.isPassed()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::string const &nickname = message.getParam();
    if (nickname.empty()) {
        return NumericReply(ERR_NONICKNAMEGIVEN) << session >> session;
    }

    if (!isNicknameValid(nickname)) {
        return NumericReply(ERR_ERRONEUSNICKNAME) << session << nickname >> session;
    }

    if (sessionRepository->find(nickname)) {
        return NumericReply(ERR_NICKNAMEINUSE) << session << nickname >> session;
    }

    if (!session.getNickname().empty()) {
        session << RPL_NICK(session.getNickname(), nickname);
    }

    session.updateNickname(nickname);
    if (!session.isRegistered() && !session.getUsername().empty()) {
        _register(session);
    }
}

bool isNicknameValid(std::string const &nickname) {
    if (nickname.size() > 9) {
        return false;
    }

    if (std::isdigit(nickname[0])) {
        return false;
    }

    for (str_iter it = nickname.begin(); it != nickname.end(); it++) {
        if (!isCharacterValid(*it)) {
            return false;
        }
    }

    return true;
}

bool isCharacterValid(char c) {
    return std::isalnum(c) ||
           c == '[' || c == ']' ||
           c == '{' || c == '}' ||
           c == '\\' || c == '|' ||
           c == '-' || c == '_';
}

std::string RPL_NICK(std::string const &oldNick, std::string const &newNick) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << oldNick << DELIMITER
       << "NICK" << DELIMITER
       << newNick << CRLF;
    return ss.str();
}
