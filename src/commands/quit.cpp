//
// Created by 이석규 on 2024. 8. 21..
//

#include "ChannelService.hpp"

#define RPL_QUIT_REASON(nickname, reason) (":" + nickname + " QUIT : " + reason + CRLF)

void ChannelService::quit(Session *session, Message const &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::string reason = message.getParam();
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ) {
        if ((it->second)->isParticipant(session)) {
            Channel *channel = it->second;
            *channel << RPL_QUIT_REASON(session->getNickname(), reason);
            if (channel->remove(session)) {
                ++it;
            } else {
                delete channel;
                it = channels.erase(it);
            }
        } else {
            ++it;
        }
    }
}