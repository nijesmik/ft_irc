//
// Created by 이석규 on 2024. 8. 21..
//

#include "ChannelService.hpp"

std::string RPL_QUIT(Session const &session, std::string const &reason);

typedef Session::Channels::const_iterator channel_iter;

void ChannelService::quit(Session *session, Message const &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::string const &reason = message.joinParams();
    Session::Channels joinedChannels = session->getJoinedChannels();
    for (channel_iter it = joinedChannels.begin(); it != joinedChannels.end(); it++) {
        Channel *channel = *it;
        size_t remain = channel->removeParticipant(session);
        if (!remain) {
            deleteChannel(channel->getName());
        } else {
            *channel << RPL_QUIT(*session, reason);
        }
    }
}

std::string RPL_QUIT(Session const &session, std::string const &reason) {
    std::stringstream ss;
    ss << MESSAGE_PREFIX << session.getAddress() << DELIMITER
       << "QUIT" << DELIMITER
       << MESSAGE_PREFIX << reason << CRLF;
    return ss.str();
}