//
// Created by 김세진 on 8/16/24.
//

#ifndef FT_IRC_CONNECTIONSERVICE_HPP
#define FT_IRC_CONNECTIONSERVICE_HPP

#include <string>
#include <arpa/inet.h> // inet_ntoa, htons
#include "Socket.hpp"
#include "SessionRepository.hpp"
#include "Session.hpp"
#include "Message.hpp"
#include "NumericReply.hpp"

class ConnectionService : public Socket {
public:
    ConnectionService(int port, std::string const &password, SessionRepository *repository);
    ~ConnectionService();

    Session *connect();
    void disconnect(Socket::fd_t sessionFd);
    Session *getSession(Socket::fd_t sessionFd);

    void unknown(Session &session, Message const &message);
    void pass(Session &session, Message const &message);
    void nick(Session &session, Message const &message);
    void user(Session &session, Message const &message);
    void ping(Session &session, Message const &message);

private:
    std::string password;
    SessionRepository *sessionRepository;

    void _register(Session &session);
};

#endif //FT_IRC_CONNECTIONSERVICE_HPP
