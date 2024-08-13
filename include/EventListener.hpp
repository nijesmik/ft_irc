//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_EVENTLISTENER_HPP
#define FT_IRC_EVENTLISTENER_HPP

#include "global.hpp"
#include <sys/event.h> // kqueue
#include <stdexcept>
#include <unistd.h> // close
#include <cstddef> // NULL

#define NCHANGES 1
#define NEVENTS 64

class EventListener {
private:
    fd_t server;
    fd_t kq;
    struct kevent *events;

public:
    EventListener(fd_t server);
    ~EventListener();
    bool listen(fd_t socket);
    int pollEvents();
    bool isConnectionEvent(int index);
};

#endif //FT_IRC_EVENTLISTENER_HPP
