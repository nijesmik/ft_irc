//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "util.hpp"
#include "Parser.hpp"
#include <sys/socket.h> // socket, bind, listen
#include <stdexcept>
#include <netinet/in.h> // struct sockaddr_in
#include <unistd.h> // socket close
#include <sys/event.h> // kqueue
#include <map> // session
#include "User.hpp"

#define NEVENTS 64

class Server {
public:
    Server(char *port, char *password);
    ~Server();
    void start();

private:
    std::string password;
    fd_t listenFd; // wait for incoming connections
    fd_t kqueueFd; // wait for events
    std::map<fd_t, User *> session;

    bool run();
    void acceptConnection();
};

#endif //FT_IRC_SERVER_HPP
