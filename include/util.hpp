//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_UTIL_HPP
#define FT_IRC_UTIL_HPP

#include <iostream>
#include <sys/event.h>
#include <cstddef>

#define NCHANGES 1

typedef int fd_t;

bool registerWithKqueue(fd_t socket, fd_t kqueue);

#endif //FT_IRC_UTIL_HPP
