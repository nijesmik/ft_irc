//
// Created by 이석규 on 2024. 8. 21..
//

#include "ChannelService.hpp"

#define RPL_QUIT_REASON(nickname, reason) (":" + nickname + " QUIT : " + reason + CRLF)

void ChannelService::quit(Session *session, Message const &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::string reason = message.joinParams();
    std::vector<Channel *> joinedChannel = session->getJoinedChannel();
    std::vector<Channel *>::iterator it;
    for (it = joinedChannel.begin(); it != joinedChannel.end(); it++) {
        Channel *channel = *it;
        *channel << RPL_QUIT_REASON(session->getNickname(), reason);
        if (channel->remove(session)) {
            delete this;
        }
    }
}
