//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_USER_HPP
#define FT_IRC_USER_HPP

#include "global.hpp"
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // inet_ntoa
#include <unistd.h> // socket close

class User {
public:
    User(fd_t fd, struct sockaddr_in const &addr);
    ~User();

private:
    fd_t fd;
};

#endif //FT_IRC_USER_HPP
