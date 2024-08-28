//
// Created by 김세진 on 8/13/24.
//

#include "EventController.hpp"

EventController::EventController(int port, std::string const &password) :
        chatService(password),
        sessionService(SessionService::init(port)),
        channelService(SessionService::instance()),
        kq(kqueue()),
        events(NULL) {
    if (kq < 0) {
        throw std::runtime_error("Error: kqueue creation failed");
    }
    listen(sessionService);
}

EventController::~EventController() {
    close(kq);
    delete sessionService;
    sessionService = NULL;
}

void EventController::listen(Socket *socket) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, socket->getFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // initialize changelist
    if (kevent(kq, changelist, NCHANGES, NULL, 0, NULL) < 0) {
        throw std::runtime_error("Error: event registration failed");
    }
}

void EventController::unlisten(Session &session) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, session.getFd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, changelist, NCHANGES, NULL, 0, NULL) < 0) {
        throw std::runtime_error("Error: event delete failed");
    }

    sessionService->disconnect(session.getFd());
}

int EventController::pollEvents() {
    struct kevent eventlist[NEVENTS];
    int nev = kevent(kq, NULL, 0, eventlist, NEVENTS, NULL);
    this->events = eventlist;
    return nev;
}

bool EventController::isConnectionEvent(Socket::fd_t eventSocketFd) {
    return eventSocketFd == sessionService->getFd();
}

bool EventController::isReadableEvent(int index) {
    return events[index].filter == EVFILT_READ;
}

void EventController::handleEvents(int nev) {
    for (int i = 0; i < nev; i++) {
        handleEvent(i);
    }
}

void EventController::handleEvent(int index) {
    Message message;
    Socket::fd_t eventSocketFd = static_cast<int>(events[index].ident);

    if (isConnectionEvent(eventSocketFd)) {
        Session *session = sessionService->connect();
        return listen(session);
    }

    if (isReadableEvent(index)) {
        Session *session = sessionService->find(eventSocketFd);
        session->read() >> message;
        handleMessages(*session, message);
    }
}

void EventController::handleMessages(Session &session, Message const &message) {
    switch (message.getCommand()) {
        case Message::UNKNOWN:
            return chatService.unknown(session, message);
        case Message::PASS:
            return chatService.pass(session, message);
        case Message::NICK:
            return chatService.nick(session, message);
        case Message::USER:
            return chatService.user(session, message);
        case Message::PING:
            return chatService.ping(session, message);
        case Message::QUIT:
            channelService.quit(&session, message);
            return unlisten(session);
        case Message::JOIN:
            return channelService.join(session, message);
        case Message::PART:
            return channelService.part(session, message);
        case Message::TOPIC:
            return channelService.topic(session, message);
    }
}