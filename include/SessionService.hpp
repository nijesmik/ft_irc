//
// Created by 김세진 on 8/14/24.
//

#ifndef FT_IRC_SESSIONSERVICE_HPP
#define FT_IRC_SESSIONSERVICE_HPP

#include <map>
#include <arpa/inet.h> // inet_ntoa, htons
#include "Socket.hpp"
#include "Session.hpp"

class SessionService : public Socket {
public:
    SessionService(int port);
    ~SessionService();

    Session *accept();
    Session *get(Socket::fd_t sessionFd);

private:
    std::map<Socket::fd_t, Session *> sessions;

    typedef std::map<Socket::fd_t, Session *>::iterator iterator;
};

#endif //FT_IRC_SESSIONSERVICE_HPP
