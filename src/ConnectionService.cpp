//
// Created by 김세진 on 8/16/24.
//

#include "ConnectionService.hpp"

ConnectionService::ConnectionService(std::string const &password) : password(password) {}

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