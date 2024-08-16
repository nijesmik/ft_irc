//
// Created by 김세진 on 8/13/24.
//

#include "EventController.hpp"

EventController::EventController(int port) :
        sessionService(port),
        kq(kqueue()),
        events(NULL) {
    if (kq < 0) {
        throw std::runtime_error("Error: kqueue creation failed");
    }
    listen(sessionService);
}

EventController::~EventController() {
    close(kq);
}

void EventController::listen(Socket const &socket) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, socket.getFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // initialize changelist
    if (kevent(kq, changelist, NCHANGES, NULL, 0, NULL) < 0) {
        throw std::runtime_error("Error: event registration failed");
    }
}

int EventController::pollEvents() {
    struct kevent eventlist[NEVENTS];
    int nev = kevent(kq, NULL, 0, eventlist, NEVENTS, NULL);
    this->events = eventlist;
    return nev;
}

bool EventController::isConnectionEvent(Socket::fd_t eventSocketFd) {
    return eventSocketFd == sessionService.getFd();
}

bool EventController::isReadableEvent(int index) {
    return events[index].filter == EVFILT_READ;
}

Session *EventController::getEventSession(int index) {
    Socket::fd_t eventSocketFd = static_cast<int>(events[index].ident);
    if (isConnectionEvent(eventSocketFd)) {
        Session *session = sessionService.accept();
        listen(*session);
        return NULL;
    }
    if (isReadableEvent(index)) {
        return sessionService.get(eventSocketFd);
    }
    return NULL;
}