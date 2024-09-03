//
// Created by 김세진 on 8/13/24.
//

#include "EventController.hpp"

EventController::EventController(int port, std::string const &password) :
        connectionService(port, password, SessionRepository::init()),
        channelService(SessionRepository::instance()),
        kq(kqueue()),
        events(NULL) {
    if (kq < 0) {
        throw std::runtime_error("Error: kqueue creation failed");
    }
    listen(connectionService);
}

EventController::~EventController() {
    close(kq);
    SessionRepository::destroy();
}

void EventController::listen(Socket const &socket) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, socket.getFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // initialize changelist
    if (kevent(kq, changelist, NCHANGES, NULL, 0, NULL) < 0) {
        throw std::runtime_error("Error: event registration failed");
    }
}

void EventController::unlisten(Session const &session) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, session.getFd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, changelist, NCHANGES, NULL, 0, NULL) < 0) {
        throw std::runtime_error("Error: event delete failed");
    }
}

void EventController::remove(Session *session, const std::string &reason) {
    channelService.quit(session, reason);
    unlisten(*session);
    connectionService.disconnect(session->getFd());
}

int EventController::pollEvents() {
    struct kevent eventlist[NEVENTS];
    int nev = kevent(kq, NULL, 0, eventlist, NEVENTS, NULL);
    this->events = eventlist;
    return nev;
}

bool EventController::isConnectionEvent(Socket::fd_t eventSocketFd) {
    return eventSocketFd == connectionService.getFd();
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
        Session *session = connectionService.connect();
        return listen(*session);
    }

    if (isReadableEvent(index)) {
        Session *session = connectionService.getSession(eventSocketFd);
        if (!session->read()) {
            return remove(session);
        }
        while (*session >> message) {
            handleMessage(session, message);
            if (message.getCommand() == Message::QUIT) {
                return;
            }
        }
    }
}

void EventController::handleMessage(Session *session, Message const &message) {
    switch (message.getCommand()) {
        case Message::UNKNOWN:
            return connectionService.unknown(*session, message);
        case Message::PASS:
            return connectionService.pass(*session, message);
        case Message::NICK:
            return connectionService.nick(*session, message);
        case Message::USER:
            return connectionService.user(*session, message);
        case Message::PING:
            return connectionService.ping(*session, message);
        case Message::QUIT:
            return remove(session, message.getParam());
        case Message::JOIN:
            return channelService.join(session, message);
        case Message::PART:
            return channelService.part(session, message);
        case Message::TOPIC:
            return channelService.topic(session, message);
        case Message::PRIVMSG:
            return channelService.privmsg(session, message);
        case Message::KICK:
            return channelService.kick(session, message);
        case Message::INVITE:
            return channelService.invite(session, message);
        case Message::MODE:
            return channelService.mode(session, message);
        case Message::WHO:
            return;
        case Message::BOT:
            return botService.makeLottery(session);
    }
}