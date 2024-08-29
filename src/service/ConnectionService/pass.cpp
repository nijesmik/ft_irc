//
// Created by 김세진 on 8/16/24.
//

#include "ConnectionService.hpp"

void ConnectionService::pass(Session &session, const Message &message) {
    std::string const &pw = message.getParam();
    if (pw.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << "PASS" >> session;
    }
    if (pw != this->password) {
        return NumericReply(ERR_PASSWDMISMATCH) >> session;
    }
    if (session.isPassed()) {
        return NumericReply(ERR_ALREADYREGISTRED) >> session;
    }
    session.setPassed();
}