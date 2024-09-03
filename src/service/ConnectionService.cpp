//
// Created by 김세진 on 8/16/24.
//

#include "ConnectionService.hpp"

ConnectionService::ConnectionService(int port, std::string const &password, SessionRepository *repository)
        : password(password),
          sessionRepository(repository) {
    setNonBlocking();
    allowReusePort();
    bind(port);
    open();
}

ConnectionService::~ConnectionService() {}

void ConnectionService::unknown(Session &session, Message const &message) {
    NumericReply(ERR_UNKNOWNCOMMAND) << session << message.getParam() >> session;
}

void ConnectionService::_register(Session &session) {
    NumericReply(RPL_WELCOME, session.getAddress()) << session >> session;
    NumericReply(RPL_YOURHOST, session.getServername()) << session >> session;
    NumericReply(RPL_CREATED) << session >> session;
    session.setRegistered();
}

Session *ConnectionService::connect() {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);

    // accept new connection
    fd_t socket = ::accept(fd, (struct sockaddr *) &address, &addressLength);
    if (socket < 0) {
        throw std::runtime_error("Error: connection accept failed");
    }

    // register new session
    std::stringstream ip;
    ip << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port);
    Session *session = new Session(socket, ip.str());
    sessionRepository->add(session);
    return session;
}

void ConnectionService::disconnect(Socket::fd_t sessionFd) {
    if (close(sessionFd) < 0) {
        std::cerr << "Error: session close failed" << std::endl;
    }

    sessionRepository->remove(sessionFd);
}

Session *ConnectionService::getSession(Socket::fd_t sessionFd) {
    return sessionRepository->find(sessionFd);
}
