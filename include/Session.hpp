//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_SESSION_HPP
#define FT_IRC_SESSION_HPP

#include  <iostream>
#include "Socket.hpp"
#include "Message.hpp"
#include "Parser.hpp"

class Session : public Socket {
public:
    Session(Socket::fd_t fd);
    ~Session();

    Session &read();
    void operator>>(Message &message);

    bool isRegistered() const;
    void signUp();

private:
    bool registered;
};

#endif //FT_IRC_SESSION_HPP
