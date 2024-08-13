//
// Created by 김세진 on 8/13/24.
//

#include "Client.hpp"

Client::Client(Socket::fd_t fd) : Socket(fd) {}

Client::~Client() {}
