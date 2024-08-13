//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

#include "global.hpp"
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // inet_ntoa
#include <unistd.h> // socket close
#include <sys/socket.h> // recv
#include <algorithm> // remove

class Client {
public:
    Client(t_socket fd, struct sockaddr_in const &addr);
    ~Client();

    std::string read();

private:
    const t_socket fd;
};

#endif //FT_IRC_CLIENT_HPP
