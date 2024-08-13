//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "Parser.hpp"
#include "EventListener.hpp"
#include <map> // session
#include "Client.hpp"
#include "Socket.hpp"

class Server {
public:
    Server(char *port, char *password);
    ~Server();
    void start();

private:
    std::string password;
    Socket connection; // wait for incoming connections
    EventListener eventListener;
    std::map<Socket::fd_t, Client *> session;

    void run();
    void handleEvents(int nev); // choose accept or read
    void acceptConnection();
    void readEventSocket(int index);
};

#endif //FT_IRC_SERVER_HPP
