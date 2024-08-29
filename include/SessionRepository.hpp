//
// Created by 김세진 on 8/14/24.
//

#ifndef FT_IRC_SESSIONREPOSITORY_HPP
#define FT_IRC_SESSIONREPOSITORY_HPP

#include <map>
#include "Session.hpp"

class SessionRepository {
public:
    static SessionRepository *init();
    static SessionRepository *instance();
    static void destroy();

    ~SessionRepository();

    void add(Session *session);
    Session *find(Socket::fd_t sessionFd);
    Session *find(std::string const &nickname);
    void remove(Socket::fd_t sessionFd);

private:
    SessionRepository();
    SessionRepository(SessionRepository const &other);
    SessionRepository &operator=(SessionRepository const &other);

    static SessionRepository *singleton;
    std::map<Socket::fd_t, Session *> sessions;

    typedef std::map<Socket::fd_t, Session *>::iterator iterator;
};

#endif //FT_IRC_SESSIONREPOSITORY_HPP
