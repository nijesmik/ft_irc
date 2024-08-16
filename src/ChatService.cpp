//
// Created by 김세진 on 8/16/24.
//

#include "ChatService.hpp"

ChatService::ChatService(std::string const &password) : password(password) {}

ChatService::~ChatService() {}

void ChatService::unknown(Session &session, Message const &message) {
    session << NumericReply::get(ERR_UNKNOWNCOMMAND, message.getParam(0));
}