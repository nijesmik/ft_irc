//
// Created by 김세진 on 8/13/24.
//

#include "Client.hpp"

Client::Client(fd_t fd, struct sockaddr_in const &addr) : fd(fd) {
    std::string ip = std::string(inet_ntoa(addr.sin_addr));
    int port = ntohs(addr.sin_port);
    std::cout << "New connection from " << ip << ":" << port << std::endl;
}

Client::~Client() {
    close(fd);
}

std::string Client::read() {
    char buffer[BUFSIZ];
    std::string result;
    ssize_t n;
    while ((n = recv(fd, buffer, BUFSIZ, 0)) > 0) {
        result.append(buffer, n);
    }
    if (n < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
        throw std::runtime_error("Error: socket read failed");
    }
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    std::cout << "Received: " << result << std::endl;
    return result;
}