//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "global.hpp"
#include "Parser.hpp"
#include <sys/socket.h> // socket, bind, listen
#include <stdexcept>
#include <netinet/in.h> // struct sockaddr_in
#include <unistd.h> // socket close
#include "EventListener.hpp"
#include <map> // session
#include "Client.hpp"

class Server {
public:
    Server(char *port, char *password);
    ~Server();
    void start();

private:
    std::string password;
    fd_t serverSocket; // wait for incoming connections
    EventListener eventListener;
    std::map<fd_t, Client *> session;

    void run();
    void handleEvents(int nev);
    void acceptConnection();
};

#endif //FT_IRC_SERVER_HPP
