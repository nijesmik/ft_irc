//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "Parser.hpp"
#include "EventManager.hpp"
#include "SessionManager.hpp"
#include "Session.hpp"
#include "Socket.hpp"

class Server {
public:
    Server(char *port, char *password);
    ~Server();
    void start();

private:
    std::string password;
    Socket connection; // wait for incoming connections
    EventManager eventManager;
    SessionManager sessionManager;

    void run();
    void handleEvents(int nev); // choose accept or read
    void acceptConnection();
};

#endif //FT_IRC_SERVER_HPP
