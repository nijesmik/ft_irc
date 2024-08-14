//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_SESSION_HPP
#define FT_IRC_SESSION_HPP

#include "Socket.hpp"

class Session : public Socket {
public:
    Session(Socket::fd_t fd);
    ~Session();
};

#endif //FT_IRC_SESSION_HPP
