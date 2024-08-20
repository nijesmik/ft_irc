//
// Created by 김세진 on 8/16/24.
//

#include "NumericReply.hpp"
#include "Server.hpp"

std::string NumericReply::message(int code) {
    switch (code) {
        case ERR_UNKNOWNCOMMAND: // 421
            return ERR_UNKNOWNCOMMAND_MESSAGE;
        case ERR_NONICKNAMEGIVEN: // 431
            return ERR_NONICKNAMEGIVEN_MESSAGE;
        case ERR_ERRONEUSNICKNAME: // 432
            return ERR_ERRONEUSNICKNAME_MESSAGE;
        case ERR_NICKNAMEINUSE: // 433
            return ERR_NICKNAMEINUSE_MESSAGE;
        case ERR_NOTREGISTERED: // 451
            return ERR_NOTREGISTERED_MESSAGE;
        case ERR_NEEDMOREPARAMS: // 461
            return ERR_NEEDMOREPARAMS_MESSAGE;
        case ERR_ALREADYREGISTRED: // 462
            return ERR_ALREADYREGISTRED_MESSAGE;
        case ERR_PASSWDMISMATCH: // 464
            return ERR_PASSWDMISMATCH_MESSAGE;
        default:
            return "";
    }
}

std::string NumericReply::message(int code, Session const &session) {
    switch (code) {
        case RPL_WELCOME: // 001
            return RPL_WELCOME_MESSAGE(Server::NETWORK_NAME, session.getAddress());
        case RPL_YOURHOST: // 002
            return RPL_YOURHOST_MESSAGE(session.getServername(), Server::VERSION);
        case RPL_CREATED: // 003
            return RPL_CREATED_MESSAGE(Server::CREATED_TIME);
        default:
            throw std::runtime_error("Error: Invalid numeric reply code");
    }
}

void NumericReply::append(std::stringstream &ss, int code) {
    ss << std::setw(3) << std::setfill('0') << code << DELIMITER;
}

void NumericReply::append(std::stringstream &ss, std::string const &str) {
    ss << str << DELIMITER;
}

void NumericReply::appendMessage(std::stringstream &ss, std::string const &message) {
    ss << MESSAGE_PREFIX << message << CRLF;
}

std::string NumericReply::get(int code) {
    std::stringstream ss;
    append(ss, code);
    appendMessage(ss, message(code));
    return ss.str();
}

std::string NumericReply::get(int code, std::string const &param) {
    std::stringstream ss;
    append(ss, code);
    append(ss, param);
    appendMessage(ss, message(code));
    return ss.str();
}

std::string NumericReply::get(int code, Session const &session) {
    std::stringstream ss;
    append(ss, code);
    append(ss, session.getNickname());
    appendMessage(ss, message(code, session));
    return ss.str();
}