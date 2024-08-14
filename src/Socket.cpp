//
// Created by 김세진 on 8/14/24.
//

#include "Socket.hpp"
#include "Session.hpp"

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

Session *Socket::accept() {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    fd_t client = ::accept(fd, (struct sockaddr *) &address, &addressLength);
    if (client < 0) {
        throw std::runtime_error("Error: connection accept failed");
    }
    std::string ip = std::string(inet_ntoa(address.sin_addr));
    int port = ntohs(address.sin_port);
    std::cout << "New connection from " << ip << ":" << port << std::endl;
    return new Session(client);
}

std::string Socket::read() {
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