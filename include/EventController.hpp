//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_EVENTCONTROLLER_HPP
#define FT_IRC_EVENTCONTROLLER_HPP

#include "Socket.hpp"
#include <sys/event.h> // kqueue
#include <unistd.h> // kqueue close
#include <cstddef> // NULL
#include "SessionRepository.hpp"
#include "ConnectionService.hpp"
#include "ChannelService.hpp"
#include "Message.hpp"
#include "BotService.hpp"

#define NCHANGES 1
#define NEVENTS 64

class EventController {
private:
    ConnectionService connectionService;
    ChannelService channelService;
    BotService botService;

    int kq;
    struct kevent *events;

    void listen(Socket const &socket);
    void unlisten(Session const &session);
    void remove(Session *session, std::string const &reason = "");

    bool isConnectionEvent(Socket::fd_t eventSocketFd);
    bool isReadableEvent(int index);
    void handleEvent(int index);
    void handleMessage(Session *session, Message const &message);

public:
    EventController(int port, std::string const &password);
    ~EventController();

    int pollEvents();
    void handleEvents(int nev);
};

#endif //FT_IRC_EVENTCONTROLLER_HPP
