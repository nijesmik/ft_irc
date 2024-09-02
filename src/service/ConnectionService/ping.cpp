#include "ConnectionService.hpp"

std::string RPL_PONG(std::string const &token);

void ConnectionService::ping(Session &session, const Message &message) {
    std::string const &token = message.getParam();
    if (token.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "PING" >> session;
    }
    session << RPL_PONG(token);
}

std::string RPL_PONG(std::string const &token) {
    std::stringstream ss;
    ss << "PONG" << DELIMITER
       << MESSAGE_PREFIX << token << CRLF;
    return ss.str();
}