//
// Created by 김세진 on 8/13/24.
//

#include "EventListener.hpp"

EventListener::EventListener(Socket const &serverConnection) :
        serverConnection(serverConnection.getFd()),
        kq(kqueue()),
        events(NULL) {
    if (kq < 0) {
        throw std::runtime_error("Error: kqueue creation failed");
    }
}

EventListener::~EventListener() {
    close(kq);
}

void EventListener::listen(Socket const &socket) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, socket.getFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // initialize changelist
    if (kevent(kq, changelist, NCHANGES, NULL, 0, NULL) < 0) {
        throw std::runtime_error("Error: event registration failed");
    }
}

int EventListener::pollEvents() {
    struct kevent eventlist[NEVENTS];
    int nev = kevent(kq, NULL, 0, eventlist, NEVENTS, NULL);
    this->events = eventlist;
    return nev;
}

bool EventListener::isConnectionEvent(int index) {
    return getEventSocket(index) == serverConnection;
}

Socket::fd_t EventListener::getEventSocket(int index) {
    return static_cast<int>(events[index].ident);
}

bool EventListener::canReadEvent(int index) {
    return events[index].filter == EVFILT_READ;
}