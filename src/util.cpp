//
// Created by 김세진 on 8/13/24.
//

#include "util.hpp"

bool registerWithKqueue(fd_t socket, fd_t kqueue) {
    struct kevent changelist[NCHANGES];
    EV_SET(changelist, socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // initialize changelist
    return kevent(kqueue, changelist, NCHANGES, NULL, 0, NULL) >= 0;
}