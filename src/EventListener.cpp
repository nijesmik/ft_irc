//
// Created by 김세진 on 8/13/24.
//

#include "EventListener.hpp"

EventListener::EventListener(fd_t server) : server(server), kq(kqueue()), events(NULL) {
    if (kq < 0) {
        throw std::runtime_error("Error: kqueue creation failed");
    }
}

EventListener::~EventListener() {
    close(kq);
}

bool EventListener::listen(fd_t socket) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // initialize changelist
    return kevent(kq, changelist, NCHANGES, NULL, 0, NULL) >= 0;
}

int EventListener::pollEvents() {
    struct kevent eventlist[NEVENTS];
    int nev = kevent(kq, NULL, 0, eventlist, NEVENTS, NULL);
    this->events = eventlist;
    return nev;
}

bool EventListener::isConnectionEvent(int index) {
    return static_cast<int>(events[index].ident) == server;
}