//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_EVENTLISTENER_HPP
#define FT_IRC_EVENTLISTENER_HPP

#include "global.hpp"
#include <sys/event.h> // kqueue
#include <unistd.h> // close
#include <cstddef> // NULL

#define NCHANGES 1
#define NEVENTS 64

class EventListener {
private:
    const t_socket serverConnection;
    t_socket kq;
    struct kevent *events;

public:
    EventListener(t_socket serverConnection);
    ~EventListener();
    bool listen(t_socket socket);
    int pollEvents();
    bool isConnectionEvent(int index);
    bool canReadEvent(int index);
    t_socket getEventSocket(int index);
};

#endif //FT_IRC_EVENTLISTENER_HPP
