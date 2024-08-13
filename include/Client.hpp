//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

#include "Socket.hpp"

class Client : public Socket {
public:
    Client(Socket::fd_t fd);
    ~Client();
};

#endif //FT_IRC_CLIENT_HPP
