//
// Created by 김세진 on 8/21/24.
//

#include "ChannelService.hpp"

#define RPL_JOIN_CHANNEL(nickname, channelName) (nickname + " JOIN " + channelName)

bool isValidChannel(const std::string &channel);

void ChannelService::join(Session &session, const Message &message) {
    if (!session.isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) >> session;
    }

    std::vector<std::string> channels = message.getSplitedParam(0, ',');
    if (channels.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "JOIN" >> session;
    }

    std::vector<std::string> keys = message.getSplitedParam(1, ',');
    std::vector<std::string>::iterator keyIt = keys.begin();
    for (std::vector<std::string>::iterator channelIt = channels.begin(); channelIt != channels.end(); ++channelIt) {
        if (!isValidChannel(*channelIt)) {
            return NumericReply(ERR_BADCHANMASK) << session << "JOIN" >> session;
        }

        Channel *channel = getChannel(*channelIt);
        channel->join(session, keyIt != keys.end() ? *keyIt : std::string());

        if (keyIt != keys.end()) {
            ++keyIt;
        }
    }
}

// 이거
void Channel::join(Session &session, const std::string &key) {
    (void) key;
    // TODO: Channel에 session이 있는지 확인 (hasSession)
    //  MODE 설정 되어있는지 확인 (각 MODE에 따라 예외)

    participants.insert(&session);

    std::string userList;
    // TODO: Operator 인지 아닌지 구분하기
    //  Channel 이 UserList를 들고 display

    // TODO: 나중에 방의 옵션(#, &)을 달아줘야 함
    *this << RPL_JOIN_CHANNEL(session.getNickname(), name);
    if (!topic.empty()) {
        NumericReply(RPL_TOPIC, this->topic) << session << name >> session;
    }
    NumericReply(RPL_NAMREPLY, userList) << session << name >> session;
    NumericReply(RPL_ENDOFNAMES, "End of /NAMES list") << session << name >> session;
}

bool isValidChannel(const std::string &channel) {
    if (channel.length() < 2 || channel.size() > CHANNELLEN) {
        return false;
    }
    return (channel[0] == '#' || channel[0] == '&');
}
