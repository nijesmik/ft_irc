//
// Created by 김세진 on 8/16/24.
//

#include "NumericReply.hpp"
#include "Server.hpp"

std::string NumericReply::message(int code) {
    switch (code) {
        case RPL_CREATED: // 003
            return RPL_CREATED_MESSAGE(Server::CREATED_TIME);
        case RPL_ENDOFWHO: // 315
            return RPL_ENDOFWHO_MESSAGE;
        case RPL_ENDOFNAMES: // 366
            return RPL_ENDOFNAMES_MESSAGE;
        case ERR_NOSUCHNICK: // 401
            return ERR_NOSUCHNICK_MESSAGE;
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
        case ERR_USERONCHANNEL: // 443
            return ERR_USERONCHANNEL_MESSAGE;
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
        case ERR_UNKNOWNMODE: // 472
            return ERR_UNKNOWNMODE_MESSAGE;
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
        case ERR_INVALIDMODEPARAM: // 696
            return ERR_INVALIDMODEPARAM_MESSAGE;
        default:
            return std::string();
    }
}

std::string NumericReply::message(int code, std::string const &param) {
    switch (code) {
        case RPL_WELCOME: // 001
            return RPL_WELCOME_MESSAGE(Server::NETWORK_NAME, param);
        case RPL_YOURHOST: // 002
            return RPL_YOURHOST_MESSAGE(param, Server::VERSION);
        case RPL_TOPIC: // 331
            return RPL_TOPIC_MESSAGE(param);
        case RPL_WHOREPLY: // 352
            return RPL_WHOREPLY_MESSAGE(param);
        case RPL_NAMREPLY: // 353
            return RPL_NAMREPLY_MESSAGE(param);
        default:
            throw std::runtime_error("Error: Invalid numeric reply code");
    }
}

void NumericReply::appendCode(int code) {
    _ss << std::setw(3) << std::setfill('0') << code << DELIMITER;
}

void NumericReply::appendMessage() {
    if (!_message.empty()) {
        _ss << MESSAGE_PREFIX << _message;
    }
    _ss << CRLF;
}

NumericReply::NumericReply(int code) : _message(message(code)) {
    appendCode(code);
}

NumericReply::NumericReply(int code, std::string const &param) : _message(message(code, param)) {
    appendCode(code);
}

NumericReply &NumericReply::operator<<(char const *str) {
    _ss << str << DELIMITER;
    return *this;
}

NumericReply &NumericReply::operator<<(std::string const &str) {
    if (str.empty()) {
        return *this;
    }
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
    appendMessage();
    std::string output = _ss.str();
    socket << output;
}

void NumericReply::operator>>(Socket *socket) {
    operator>>(*socket);
}

void NumericReply::operator>>(Channel &channel) {
    appendMessage();
    channel << _ss.str();
}

void NumericReply::operator>>(Channel *channel) {
    operator>>(*channel);
}
