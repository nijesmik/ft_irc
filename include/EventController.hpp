//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_EVENTCONTROLLER_HPP
#define FT_IRC_EVENTCONTROLLER_HPP

#include "Socket.hpp"
#include <sys/event.h> // kqueue
#include <unistd.h> // kqueue close
#include <cstddef> // NULL
#include "SessionService.hpp"

#define NCHANGES 1
#define NEVENTS 64

class EventController {
private:
    SessionService sessionService;
    int kq;
    struct kevent *events;

    bool isConnectionEvent(Socket::fd_t eventSocketFd);
    bool isReadableEvent(int index);

public:
    EventController(int port);
    ~EventController();
    void listen(Socket const &socket);
    int pollEvents();
    Session *getEventSession(int index);
};

#endif //FT_IRC_EVENTCONTROLLER_HPP
