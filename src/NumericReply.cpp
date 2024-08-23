//
// Created by 김세진 on 8/16/24.
//

#include "NumericReply.hpp"
#include "Server.hpp"

std::string NumericReply::message(int code) {
    switch (code) {
        case ERR_NOSUCHCHANNEL: // 403
            return ERR_NOSUCHCHANNEL_MESSAGE;
        case ERR_UNKNOWNCOMMAND: // 421
            return ERR_UNKNOWNCOMMAND_MESSAGE;
        case ERR_NONICKNAMEGIVEN: // 431
            return ERR_NONICKNAMEGIVEN_MESSAGE;
        case ERR_ERRONEUSNICKNAME: // 432
            return ERR_ERRONEUSNICKNAME_MESSAGE;
        case ERR_NICKNAMEINUSE: // 433
            return ERR_NICKNAMEINUSE_MESSAGE;
        case ERR_USERNOTINCHANNEL: // 441
            return ERR_USERNOTINCHANNEL_MESSAGE;
        case ERR_NOTONCHANNEL: // 442
            return ERR_NOTONCHANNEL_MESSAGE;
        case ERR_NOTREGISTERED: // 451
            return ERR_NOTREGISTERED_MESSAGE;
        case ERR_NEEDMOREPARAMS: // 461
            return ERR_NEEDMOREPARAMS_MESSAGE;
        case ERR_ALREADYREGISTRED: // 462
            return ERR_ALREADYREGISTRED_MESSAGE;
        case ERR_PASSWDMISMATCH: // 464
            return ERR_PASSWDMISMATCH_MESSAGE;
        case ERR_CHANNELISFULL: // 471
            return ERR_CHANNELISFULL_MESSAGE;
        case ERR_INVITEONLYCHAN: // 473
            return ERR_INVITEONLYCHAN_MESSAGE;
        case ERR_BANNEDFROMCHAN: // 474
            return ERR_BANNEDFROMCHAN_MESSAGE;
        case ERR_BADCHANNELKEY: // 475
            return ERR_BADCHANNELKEY_MESSAGE;
        case ERR_BADCHANMASK: // 476
            return ERR_BADCHANMASK_MESSAGE;
        case ERR_CHANOPRIVSNEEDED: // 482
            return ERR_CHANOPRIVSNEEDED_MESSAGE;
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

std::string NumericReply::message(int code, std::string const &param) {
    switch (code) {
        case RPL_TOPIC: // 331
            return RPL_TOPIC_MESSAGE(param);
        case RPL_NAMREPLY: // 353
            return RPL_NAMREPLY_MESSAGE(param);
        case RPL_ENDOFNAMES: //366
            return RPL_ENDOFNAMES_MESSAGE(param);
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

std::string NumericReply::channelReply(int code, std::string const &nickname, std::string const &channelName) {
    std::stringstream ss;
    append(ss, code);
    append(ss, nickname);
    append(ss, channelName);
    appendMessage(ss, message(code));
    return ss.str();
}

NumericReply::NumericReply(int code) : _message(message(code)) {
    _ss << std::setw(3) << std::setfill('0') << code << DELIMITER;
}

NumericReply::NumericReply(int code, std::string const  &param) : _message(message(code, param)) {
    _ss << std::setw(3) << std::setfill('0') << code << DELIMITER;
}

NumericReply &NumericReply::operator<<(char const *str) {
    _ss << str << DELIMITER;
    return *this;
}

NumericReply &NumericReply::operator<<(std::string const &str) {
    _ss << str << DELIMITER;
    return *this;
}

NumericReply &NumericReply::operator<<(Session const &session) {
    return operator<<(session.getNickname());
}

NumericReply &NumericReply::operator<<(Session *session) {
    return operator<<(*session);
}

void NumericReply::operator>>(Socket &socket) {
    _ss << MESSAGE_PREFIX << _message << CRLF;
    socket << _ss.str();
}

void NumericReply::operator>>(Socket *socket) {
    operator>>(*socket);
}

void NumericReply::operator>>(Channel &channel) {
    _ss << MESSAGE_PREFIX << _message << CRLF;
    channel << _ss.str();
}

void NumericReply::operator>>(Channel *channel) {
    operator>>(*channel);
}