//
// Created by 김세진 on 8/21/24.
//

#include "ChatService.hpp"

void ChatService::user(Session &session, Message const &message) {
    if (!session.isPassed()) {
        return session << NumericReply::get(ERR_NOTREGISTERED);
    }
    if (session.isRegistered()) {
        return session << NumericReply::get(ERR_ALREADYREGISTRED);
    }

    std::string realname = message.getParamsAll(3, ':');
    if (realname.empty()) {
        return session << NumericReply::get(ERR_NEEDMOREPARAMS, "USER");
    }

    session.updateUser(message.getParam(0), message.getParam(1), message.getParam(2), realname);
    if (!session.isRegistered() && !session.getNickname().empty()) {
        _register(session);
    }
}