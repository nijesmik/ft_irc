//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "Parser.hpp"
#include <string>

class Server {
public:
    Server(char *port, char *password);

private:
    int port;
    std::string password;
};

#endif //FT_IRC_SERVER_HPP
