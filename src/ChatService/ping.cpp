#include "ChatService.hpp"

void ChatService::ping(Session &session, const Message &message) {
    std::string token = message.getParam();
    if (token.empty()) {
        return session << NumericReply::get(ERR_NEEDMOREPARAMS, "PING");
    }
}