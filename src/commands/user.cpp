//
// Created by 김세진 on 8/21/24.
//

#include "ChatService.hpp"

void ChatService::user(Session &session, Message const &message) {
    if (!session.isPassed()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }
    if (session.isRegistered()) {
        return NumericReply(ERR_ALREADYREGISTRED) << session >> session;
    }

    std::string const &realname = message.joinParams(3, ':');
    if (realname.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << "USER" >> session;
    }

    session.updateUser(message.getParam(0), message.getParam(1), message.getParam(2), realname);
    if (!session.isRegistered() && !session.getNickname().empty()) {
        _register(session);
    }
}