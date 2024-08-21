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
#include "ChatService.hpp"
#include "ChannelService.hpp"
#include "Message.hpp"

#define NCHANGES 1
#define NEVENTS 64

class EventController {
private:
    ChatService chatService;
    SessionService *sessionService;
    ChannelService channelService;

    int kq;
    struct kevent *events;

    bool isConnectionEvent(Socket::fd_t eventSocketFd);
    bool isReadableEvent(int index);
    void handleEvent(int index);
    void handleMessages(Session &session, Message const &message);

public:
    EventController(int port, std::string const &password);
    ~EventController();

    void listen(Socket *socket);
    void unlisten(Session &session);
    int pollEvents();
    void handleEvents(int nev);
};

#endif //FT_IRC_EVENTCONTROLLER_HPP
