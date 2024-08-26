//
// Created by 김세진 on 8/14/24.
//

#ifndef FT_IRC_SESSIONREPOSITORY_HPP
#define FT_IRC_SESSIONREPOSITORY_HPP

#include <map>
#include <arpa/inet.h> // inet_ntoa, htons
#include "Socket.hpp"
#include "Session.hpp"

class SessionRepository : public Socket {
public:
    static SessionRepository *init(int port);
    static SessionRepository *instance();

    ~SessionRepository();

    Session *connect();
    Session *find(Socket::fd_t sessionFd);
    Session *find(std::string const &nickname);
    void disconnect(Socket::fd_t sessionFd);

private:
    SessionRepository(int port);
    SessionRepository(SessionRepository const &other);
    SessionRepository &operator=(SessionRepository const &other);

    static SessionRepository *singleton;
    std::map<Socket::fd_t, Session *> sessions;

    typedef std::map<Socket::fd_t, Session *>::iterator iterator;
};

#endif //FT_IRC_SESSIONREPOSITORY_HPP
