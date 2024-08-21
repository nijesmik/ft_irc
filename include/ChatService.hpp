//
// Created by 김세진 on 8/16/24.
//

#ifndef FT_IRC_CHATSERVICE_HPP
#define FT_IRC_CHATSERVICE_HPP

#include <string>
#include "Session.hpp"
#include "Message.hpp"
#include "NumericReply.hpp"

class ChatService {
public:
    ChatService(std::string const &password);
    ~ChatService();

    void unknown(Session &session, Message const &message);
    void pass(Session &session, Message const &message);
    void nick(Session &session, Message const &message);
    void user(Session &session, Message const &message);
    void ping(Session &session, Message const &message);
    void quit(Session &session, Message const &message);

private:
    std::string password;

    void _register(Session &session);
};

#endif //FT_IRC_CHATSERVICE_HPP
