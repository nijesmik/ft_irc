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

#define RPL_NOTOPIC 331
#define RPL_NOTOPIC_MESSAGE "No topic is set"

#define RPL_TOPIC 332
#define RPL_TOPIC_MESSAGE(channelTopic) (channelTopic)

#define RPL_NAMREPLY 353
#define RPL_NAMREPLY_MESSAGE(userList) (userList)

#define RPL_ENDOFNAMES 366
#define RPL_ENDOFNAMES_MESSAGE(param) (param)

#define ERR_NOSUCHCHANNEL 403
#define ERR_NOSUCHCHANNEL_MESSAGE "No such channel"

#define ERR_TOOMANYCHANNELS 405
#define ERR_TOOMANYCHANNEL_MESSAGE "You have joined too many channels"

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

#define ERR_CHANNELISFULL 471
#define ERR_CHANNELISFULL_MESSAGE "Cannot join channel (+l)"

#define ERR_INVITEONLYCHAN 473
#define ERR_INVITEONLYCHAN_MESSAGE "Cannot join channel (+i)"

#define ERR_BANNEDFROMCHAN 474
#define ERR_BANNEDFROMCHAN_MESSAGE "Cannot join channel (+b)"

#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANNELKEY_MESSAGE "Cannot join channel (+k)"

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
    NumericReply(int code, std::string const  &param);

    NumericReply &operator<<(char const *str);
    NumericReply &operator<<(std::string const &str);
    NumericReply &operator<<(Session const &session);
    NumericReply &operator<<(Session *session);
    void operator>>(Socket &socket);
    void operator>>(Socket *socket);
    void operator>>(Channel &channel);
    void operator>>(Channel *channel);

    static std::string get(int code);
    static std::string get(int code, std::string const &param);
    static std::string get(int code, Session const &session);
    static std::string channelReply(int code, std::string const &nickname, std::string const &channelName);
    static std::string channelReply(int code, std::string const &nickname, std::string const &channelName,
                             std::string const &param);

private:
    const std::string _message;
    std::stringstream _ss;

    static std::string message(int code);
    static std::string message(int code, Session const &session);
    std::string message(int code, std::string const &param);
    static std::string message(int code, const std::string& nickname, const std::string& channelName, const std::string& param);
    static void append(std::stringstream &ss, int code);
    static void append(std::stringstream &ss, std::string const &str);
    static void appendMessage(std::stringstream &ss, std::string const &message);
};

#endif //FT_IRC_NUMERICREPLY_HPP
