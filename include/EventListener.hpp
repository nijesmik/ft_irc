//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_EVENTLISTENER_HPP
#define FT_IRC_EVENTLISTENER_HPP

#include "Socket.hpp"
#include <sys/event.h> // kqueue
#include <unistd.h> // kqueue close
#include <cstddef> // NULL

#define NCHANGES 1
#define NEVENTS 64

class EventListener {
private:
    const Socket::fd_t serverConnection;
    int kq;
    struct kevent *events;

public:
    EventListener(Socket const &serverConnection);
    ~EventListener();
    void listen(Socket const &socket);
    int pollEvents();
    bool isConnectionEvent(int index);
    bool canReadEvent(int index);
    Socket::fd_t getEventSocket(int index);
};

#endif //FT_IRC_EVENTLISTENER_HPP
