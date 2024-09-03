//
// Created by 김세진 on 8/22/24.
//

#include "ChannelService.hpp"

typedef std::vector<std::string>::const_iterator str_iter;

std::string RPL_KICK(Session *_operator, std::string const &channel, std::string const &user,
                     std::string const &comment);

void ChannelService::kick(Session *session, const Message &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::vector<std::string> users = message.splitParam(1, ',');
    if (users.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "KICK" >> session;
    }

    std::string const &channelName = message.getParam(0);
    Channel *channel = findChannel(channelName);
    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }
    if (!channel->isParticipant(session)) {
        return NumericReply(ERR_NOTONCHANNEL) << session << channelName >> session;
    }
    if (!channel->isOperator(session)) {
        return NumericReply(ERR_CHANOPRIVSNEEDED) << session << channelName >> session;
    }

    std::string const &comment = message.getParam(2);
    for (str_iter user = users.begin(); user != users.end(); user++) {
        Session *participant = channel->getParticipant(*user);
        if (!participant) {
            NumericReply(ERR_USERNOTINCHANNEL) << session << *user << channelName >> session;
        } else {
            *channel << RPL_KICK(session, channelName, *user, comment);
            channel->removeParticipant(participant);
        }
    }
}

std::string RPL_KICK(Session *_operator, std::string const &channel, std::string const &user,
                     std::string const &comment) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << _operator->getAddress() << DELIMITER
       << "KICK" << DELIMITER
       << channel << DELIMITER
       << user;
    if (!comment.empty()) {
        ss << DELIMITER << MESSAGE_PREFIX << comment;
    }
    ss << CRLF;
    return ss.str();
}