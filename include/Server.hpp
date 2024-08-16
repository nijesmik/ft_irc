//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "Parser.hpp"
#include "EventController.hpp"
#include "Session.hpp"
#include "Socket.hpp"

class Server {
public:
    Server(char *port, char *password);
    ~Server();
    void start();

private:
    std::string password;
    EventController eventController;

    void run();
    void handleEvents(int nev); // choose accept or read
};

#endif //FT_IRC_SERVER_HPP
