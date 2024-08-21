//
// Created by 김세진 on 8/16/24.
//

#ifndef FT_IRC_NUMERICREPLY_HPP
#define FT_IRC_NUMERICREPLY_HPP

#define DELIMITER " "
#define MESSAGE_PREFIX ":"
#define CRLF "\r\n"

#define RPL_WELCOME 1
#define RPL_WELCOME_MESSAGE(networkName, clientAddress) ("Welcome to the " + networkName + " Network, " + clientAddress)

#define RPL_YOURHOST 2
#define RPL_YOURHOST_MESSAGE(serverName, version) ("Your host is " + serverName + ", running version " + version)

#define RPL_CREATED 3
#define RPL_CREATED_MESSAGE(createdTime) ("This server was created " + createdTime)

#define ERR_NOSUCHCHANNEL 403
#define ERR_NOSUCHCHANNEL_MESSAGE "No such channel"

#define ERR_UNKNOWNCOMMAND 421
#define ERR_UNKNOWNCOMMAND_MESSAGE "Unknown command"

#define ERR_NONICKNAMEGIVEN 431
#define ERR_NONICKNAMEGIVEN_MESSAGE "No nickname given"

#define ERR_ERRONEUSNICKNAME 432
#define ERR_ERRONEUSNICKNAME_MESSAGE "Erroneous nickname"

#define ERR_NICKNAMEINUSE 433
#define ERR_NICKNAMEINUSE_MESSAGE "Nickname is already in use"

#define ERR_USERNOTINCHANNEL 441 // "<client> <nick> <channel> :They aren't on that channel"
#define ERR_USERNOTINCHANNEL_MESSAGE "They aren't on that channel"

#define ERR_NOTONCHANNEL 442
#define ERR_NOTONCHANNEL_MESSAGE "You're not on that channel"

#define ERR_NOTREGISTERED 451
#define ERR_NOTREGISTERED_MESSAGE "You have not registered"

#define ERR_NEEDMOREPARAMS 461
#define ERR_NEEDMOREPARAMS_MESSAGE "Not enough parameters"

#define ERR_ALREADYREGISTRED 462
#define ERR_ALREADYREGISTRED_MESSAGE "You may not reregister"

#define ERR_PASSWDMISMATCH 464
#define ERR_PASSWDMISMATCH_MESSAGE "Password incorrect"

#define ERR_BADCHANMASK 476
#define ERR_BADCHANMASK_MESSAGE "Bad Channel Mask"

#define ERR_CHANOPRIVSNEEDED 482 // "<client> <channel> :You're not channel operator"
#define ERR_CHANOPRIVSNEEDED_MESSAGE "You're not channel operator"

#include <sstream>
#include <iomanip>
#include "Session.hpp"

class NumericReply {
public:
    NumericReply(int code);

    NumericReply &operator<<(std::string const &str);
    NumericReply &operator<<(Session const &session);
    NumericReply &operator<<(Session *session);
    void operator>>(Socket &socket);
    void operator>>(Socket *socket);

    static std::string get(int code);
    static std::string get(int code, std::string const &param);
    static std::string get(int code, Session const &session);
    static std::string channelReply(int code, std::string const &nickname, std::string const &channelName);

private:
    const std::string _message;
    std::stringstream _ss;

    static std::string message(int code);
    static std::string message(int code, Session const &session);
    static void append(std::stringstream &ss, int code);
    static void append(std::stringstream &ss, std::string const &str);
    static void appendMessage(std::stringstream &ss, std::string const &message);
};

#endif //FT_IRC_NUMERICREPLY_HPP
