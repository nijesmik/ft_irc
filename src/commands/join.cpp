//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

typedef std::vector<std::string>::const_iterator str_iter;
std::string RPL_JOIN(Session const &session, std::string const &channelName);
bool isValidChannel(const std::string &channel);

void ChannelService::join(Session &session, const Message &message) {
    if (!session.isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::vector<std::string> channelNames = message.splitParam(0, ',');
    if (channelNames.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "JOIN" >> session;
    }

    std::vector<std::string> keys = message.splitParam(1, ',');
    str_iter keyIt = keys.begin();
    str_iter channelIt = channelNames.begin();
    for (size_t i = 0; i < channelNames.size(); i++) {
        std::string const &channelName = *(channelIt + i);
        if (!isValidChannel(channelName)) {
            return NumericReply(ERR_BADCHANMASK) << session << "JOIN" >> session;
        }

        Channel *channel = findChannel(channelName);
        if (!channel) {
            channel = createChannel(channelName, &session);
        }
        channel->join(&session, i < keys.size() ? *(keyIt + i) : std::string());
    }
}

void Channel::join(Session *session, const std::string &key) {
    if (!isParticipant(session)) {
        return NumericReply(ERR_USERONCHANNEL) << session << name >> session;
    }

    if (limit && participants.size() >= limit) {
        return NumericReply(ERR_CHANNELISFULL) << session << name >> session;
    }
    if (inviteOnly) {
        return NumericReply(ERR_INVITEONLYCHAN) << session << name >> session;
    }
    if (!channelKey.empty() && channelKey != key) {
        return NumericReply(ERR_BADCHANNELKEY) << session << name >> session;
    }

    addParticipant(session);

    std::string userList;
    for (std::set<Session *>::iterator it = participants.begin(); it != participants.end();) {
        if (isOperator(*it)) {
            userList += "@" + (*it)->getNickname() + " ";
        } else {
            userList += (*it)->getNickname() + " ";
        }
    }

    *this << RPL_JOIN(*session, name);
    if (!channelTopic.empty()) {
        NumericReply(RPL_TOPIC, this->channelTopic) << session << name >> session;
    }
    NumericReply(RPL_NAMREPLY, userList) << session << name >> session;
    NumericReply(RPL_ENDOFNAMES) << session << name >> session;
}

bool isValidChannel(const std::string &channel) {
    if (channel.length() < 2 || channel.size() > CHANNELLEN) {
        return false;
    }
    return (channel[0] == '#' || channel[0] == '&');
}

std::string RPL_JOIN(Session const &session, std::string const &channelName) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << session.getAddress() << DELIMITER
       << "JOIN" << DELIMITER
       << channelName << CRLF;
    return ss.str();
}