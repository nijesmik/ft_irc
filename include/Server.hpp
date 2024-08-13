//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "global.hpp"
#include "Parser.hpp"
#include <sys/socket.h> // socket, bind, listen
#include <netinet/in.h> // struct sockaddr_in
#include <unistd.h> // socket close
#include "EventListener.hpp"
#include <map> // session
#include "Client.hpp"
#include <fcntl.h> // non-blocking

class Server {
public:
    Server(char *port, char *password);
    ~Server();
    void start();

private:
    std::string password;
    t_socket connection; // wait for incoming connections
    EventListener eventListener;
    std::map<t_socket, Client *> session;

    void run();
    void handleEvents(int nev); // choose accept or read
    void acceptConnection();
    void readEventSocket(int index);
};

#endif //FT_IRC_SERVER_HPP
