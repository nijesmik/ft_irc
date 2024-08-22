//
// Created by 김세진 on 8/16/24.
//

#include "ChatService.hpp"

void ChatService::pass(Session &session, const Message &message) {
    std::string pw = message.getParam();
    if (pw.empty()) {
        return session << NumericReply::get(ERR_NEEDMOREPARAMS, "PASS");
    }
    if (pw != this->password) {
        return session << NumericReply::get(ERR_PASSWDMISMATCH);
    }
    if (session.isPassed()) {
        return session << NumericReply::get(ERR_ALREADYREGISTRED);
    }
    session.setPassed();
}