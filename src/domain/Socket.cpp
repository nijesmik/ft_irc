//
// Created by 김세진 on 8/14/24.
//

#include "Socket.hpp"
#include <iostream>

Socket::Socket() : fd(socket(AF_INET, SOCK_STREAM, 0)) {
    if (fd < 0) {
        throw std::runtime_error("Error: socket creation failed");
    }
}

Socket::Socket(fd_t fd) : fd(fd) {}

Socket::~Socket() {
    close(fd);
}

Socket::Socket(Socket const &other) : fd(other.fd) {}

const Socket::fd_t &Socket::getFd() const {
    return fd;
}

void Socket::setNonBlocking() {
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        throw std::runtime_error("Error: socket non-blocking failed");
    }
}

void Socket::allowReusePort() {
    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void Socket::bind(int port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // open on all interfaces
    address.sin_port = htons(port); // convert to network byte order and set port
    if (::bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        throw std::runtime_error("Error: socket binding failed");
    }
}

void Socket::open() {
    if (::listen(fd, SOMAXCONN) < 0) {
        throw std::runtime_error("Error: socket listening failed");
    }
}

bool Socket::read() {
    char buffer[BUFSIZ];
    ssize_t n = recv(fd, buffer, BUFSIZ, 0);
    if (n > 0) {
        readData.write(buffer, n);
        return true;
    }
    if (n == 0) {
        std::cout << "Client disconnected" << std::endl;
    } else if (n < 0) {
        std::cerr << "Error: socket read failed" << std::endl;
    }
    return false;
}

bool Socket::operator>>(std::stringstream &stream) {
    std::string buffer;
    while (std::getline(readData, buffer, CR)) {
        if (readData.eof()) {
            readData.clear();
            readData << stream.str() << buffer;
            return false;
        }
        stream << buffer;
        if (readData.peek() == LF) {
            readData.ignore(1, LF);
            return true;
        }
        stream << CR;
    }
    readData.clear();
    return false;
}

void Socket::operator<<(std::string const &message) {
    if (send(fd, message.c_str(), message.size(), 0) < 0) {
        throw std::runtime_error("Error: socket write failed");
    }
}
