//
// Created by 김세진 on 8/16/24.
//

#ifndef FT_IRC_NUMERICREPLY_HPP
#define FT_IRC_NUMERICREPLY_HPP

#define DELIMITER " "
#define MESSAGE_PREFIX ":"
#define OPERATOR_PREFIX "@"
#define PUBLIC_CHANNEL_SYMBOL "="
#define CRLF "\r\n"

#define RPL_WELCOME 1
#define RPL_WELCOME_MESSAGE(networkName, clientAddress) ("Welcome to the " + networkName + " Network, " + clientAddress)

#define RPL_YOURHOST 2
#define RPL_YOURHOST_MESSAGE(serverName, version) ("Your host is " + serverName + ", running version " + version)

#define RPL_CREATED 3
#define RPL_CREATED_MESSAGE(createdTime) ("This server was created " + createdTime)

#define RPL_ENDOFWHO 315 // "<client> <channel> :End of WHO list"
#define RPL_ENDOFWHO_MESSAGE "End of WHO list"

#define RPL_CHANNELMODEIS 324 // "<client> <channel> <modestring> <mode arguments>..."

#define RPL_NOTOPIC 331
#define RPL_NOTOPIC_MESSAGE "No topic is set"

#define RPL_TOPIC 332 // "<client> <channel> :<topic>"
#define RPL_TOPIC_MESSAGE(channelTopic) (channelTopic)

#define RPL_INVITING 341 // "<client> <nick> <channel>"

#define RPL_WHOREPLY 352 // "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
#define RPL_WHOREPLY_MESSAGE(userList) (userList)

#define RPL_NAMREPLY 353 // "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
#define RPL_NAMREPLY_MESSAGE(userList) (userList)

#define RPL_ENDOFNAMES 366 // "<client> <channel> :End of /NAMES list"
#define RPL_ENDOFNAMES_MESSAGE "End of /NAMES list"

#define ERR_NOSUCHNICK 401 // "<client> <nickname> :No such nick/channel"
#define ERR_NOSUCHNICK_MESSAGE "No such nick/channel"

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

#define ERR_USERONCHANNEL 443 // "<client> <nick> <channel> :is already on channel"
#define ERR_USERONCHANNEL_MESSAGE "is already on channel"

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

#define ERR_UNKNOWNMODE 472 // "<client> <modechar> :is unknown setMode char to me"
#define ERR_UNKNOWNMODE_MESSAGE "is unknown setMode char to me"

#define ERR_INVITEONLYCHAN 473
#define ERR_INVITEONLYCHAN_MESSAGE "Cannot join channel (+i)"

#define ERR_BANNEDFROMCHAN 474
#define ERR_BANNEDFROMCHAN_MESSAGE "Cannot join channel (+b)"

#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANNELKEY_MESSAGE "Cannot join channel (+k)"

#define ERR_BADCHANMASK 476 // "<channel> :Bad Channel Mask"
#define ERR_BADCHANMASK_MESSAGE "Bad Channel Mask"

#define ERR_CHANOPRIVSNEEDED 482 // "<client> <channel> :You're not channel operator"
#define ERR_CHANOPRIVSNEEDED_MESSAGE "You're not channel operator"

#define ERR_INVALIDMODEPARAM 696 // "<client> <target chan/user> <mode char> <parameter> :<description>"
#define ERR_INVALIDMODEPARAM_MESSAGE "Invalid parameter for mode"

#include <sstream>
#include <iomanip>
#include "Session.hpp"

class NumericReply {
public:
    NumericReply(int code);
    NumericReply(int code, std::string const &param);

    NumericReply &operator<<(char const *str);
    NumericReply &operator<<(std::string const &str);
    NumericReply &operator<<(Session const &session);
    NumericReply &operator<<(Session *session);
    void operator>>(Socket &socket);
    void operator>>(Socket *socket);
    void operator>>(Channel &channel);
    void operator>>(Channel *channel);

private:
    const std::string _message;
    std::stringstream _ss;

    std::string message(int code);
    std::string message(int code, std::string const &param);
    void appendCode(int code);
    void appendMessage();
};

#endif //FT_IRC_NUMERICREPLY_HPP
