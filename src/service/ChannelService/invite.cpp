//
// Created by 김세진 on 8/26/24.
//

#include "ChannelService.hpp"

void ChannelService::invite(Session *session, const Message &message) {
    if (!session->isRegistered()) {
        return NumericReply(ERR_NOTREGISTERED) << session >> session;
    }

    std::string const &channelName = message.getParam(1);
    if (channelName.empty()) {
        return NumericReply(ERR_NEEDMOREPARAMS) << session << "INVITE" >> session;
    }

    Channel *channel = findChannel(channelName);
    if (!channel) {
        return NumericReply(ERR_NOSUCHCHANNEL) << session << channelName >> session;
    }

    if (!channel->isParticipant(session)) {
        return NumericReply(ERR_NOTONCHANNEL) << session << channelName >> session;
    }

    if (channel->isInviteOnly() && !channel->isOperator(session)) {
        return NumericReply(ERR_CHANOPRIVSNEEDED) << session << channelName >> session;
    }

    std::string const &nickname = message.getParam(0);
    Session *target = sessionRepository->find(nickname);
    if (!target) {
        return NumericReply(ERR_NOSUCHNICK) << session << nickname >> session;
    }

    if (channel->isParticipant(target)) {
        return NumericReply(ERR_USERONCHANNEL) << session << nickname << channelName >> session;
    }

    channel->addParticipant(session);
    NumericReply(RPL_INVITING) << session << nickname << channelName >> channel;
}