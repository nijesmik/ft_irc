//
// Created by 김세진 on 8/16/24.
//

#include "NumericReply.hpp"

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

void NumericReply::append(std::stringstream &ss, int num) {
    ss << num << DELIMITER;
}

void NumericReply::append(std::stringstream &ss, std::string const &str) {
    ss << str << DELIMITER;
}

void NumericReply::appendMessage(std::stringstream &ss, int code) {
    ss << MESSAGE_PREFIX << message(code) << CRLF;
}

std::string NumericReply::get(int code) {
    std::stringstream ss;
    append(ss, code);
    appendMessage(ss, code);
    return ss.str();
}

std::string NumericReply::get(int code, std::string const &param) {
    std::stringstream ss;
    append(ss, code);
    append(ss, param);
    appendMessage(ss, code);
    return ss.str();
}