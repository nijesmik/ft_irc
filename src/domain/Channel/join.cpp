//
// Created by 김세진 on 8/29/24.
//

#include "Channel.hpp"

std::string RPL_JOIN(Session const &session, std::string const &channelName);

void Channel::join(Session *session, const std::string &key) {
    if (isParticipant(session)) {
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

    *this << RPL_JOIN(*session, name);
    if (!channelTopic.empty()) {
        NumericReply(RPL_TOPIC, this->channelTopic) << session << name >> session;
    }
    NumericReply(RPL_NAMREPLY, getParticipantList()) << session << PUBLIC_CHANNEL_SYMBOL << name >> session;
    NumericReply(RPL_ENDOFNAMES) << session << name >> session;
}

std::string RPL_JOIN(Session const &session, std::string const &channelName) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << session.getAddress() << DELIMITER
       << "JOIN" << DELIMITER
       << channelName << CRLF;
    return ss.str();
}