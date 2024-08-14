//
// Created by 김세진 on 8/13/24.
//

#include "Session.hpp"

Session::Session(Socket::fd_t fd) : Socket(fd) {}

Session::~Session() {}
