//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_EVENTMANAGER_HPP
#define FT_IRC_EVENTMANAGER_HPP

#include "Socket.hpp"
#include <sys/event.h> // kqueue
#include <unistd.h> // kqueue close
#include <cstddef> // NULL

#define NCHANGES 1
#define NEVENTS 64

class EventManager {
private:
    const Socket::fd_t serverConnection;
    int kq;
    struct kevent *events;

public:
    EventManager(Socket const &serverConnection);
    ~EventManager();
    void listen(Socket const &socket);
    int pollEvents();
    bool isConnectionEvent(int index);
    bool isReadableEvent(int index);
    Socket::fd_t getEventSocket(int index);
};

#endif //FT_IRC_EVENTMANAGER_HPP
