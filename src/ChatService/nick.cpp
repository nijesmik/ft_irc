//
// Created by 김세진 on 8/18/24.
//

#include "ChatService.hpp"
#include "SessionService.hpp"

typedef std::string::const_iterator str_iter;
bool isCharacterValid(char c);
bool isNicknameValid(std::string const &nickname);
bool isNicknameDuplicate(std::string const &nickname);

void ChatService::nick(Session &session, const Message &message) {
    if (!session.isRegistered()) {
        return session << NumericReply::get(ERR_NOTREGISTERED);
    }

    std::string nickname = message.getParam();
    if (nickname.empty()) {
        return session << NumericReply::get(ERR_NONICKNAMEGIVEN);
    }

    if (!isNicknameValid(nickname)) {
        return session << NumericReply::get(ERR_ERRONEUSNICKNAME, nickname);
    }

    if (isNicknameDuplicate(nickname)) {
        return session << NumericReply::get(ERR_NICKNAMEINUSE, nickname);
    }

    session.updateNickname(nickname);
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

bool isNicknameDuplicate(std::string const &nickname) {
    return SessionService::instance()->find(nickname) != NULL;
}