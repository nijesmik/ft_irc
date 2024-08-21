#include "ChatService.hpp"

#define RPL_PONG(params) ("PONG :" + params + CRLF)

void ChatService::ping(Session &session, const Message &message) {
    std::string token = message.getParam();
    if (token.empty()) {
        return session << NumericReply::get(ERR_NEEDMOREPARAMS, "PING");
    } else {
        session << RPL_PONG(token);
     }
}