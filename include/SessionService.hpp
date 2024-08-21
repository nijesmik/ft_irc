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
    static SessionService *init(int port);
    static SessionService *instance();

    ~SessionService();

    Session *connect();
    Session *find(Socket::fd_t sessionFd);
    Session *find(std::string const &nickname);
    void disconnect(Socket::fd_t sessionFd);

private:
    SessionService(int port);
    SessionService(SessionService const &other);
    SessionService &operator=(SessionService const &other);

    static SessionService *singleton;
    std::map<Socket::fd_t, Session *> sessions;

    typedef std::map<Socket::fd_t, Session *>::iterator iterator;
};

#endif //FT_IRC_SESSIONSERVICE_HPP
