//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include <iostream>
#include "Parser.hpp"
#include "EventController.hpp"

class Server {
public:
    static const std::string NETWORK_NAME;
    static const std::string VERSION;
    static const std::string CREATED_TIME;

    Server(char *port, char *password);
    ~Server();
    void start();

private:
    EventController eventController;

    void run();
};

#endif //FT_IRC_SERVER_HPP
