//
// Created by 이석규 on 2024. 8. 28..
//

#include "Channel.hpp"

void Channel::setTopic(Session *session, std::string const &topicName) {
    this->channelTopic = topicName;
    NumericReply(RPL_TOPIC, this->channelTopic) << session << this->name >> this;
}

void Channel::displayTopic(Session *session) {
    if (channelTopic.empty()) {
        return NumericReply(RPL_NOTOPIC) << session << name >> session;
    }
    return NumericReply(RPL_TOPIC, this->channelTopic) << session << name >> session;
}

void Channel::topic(Session *session, const std::string &topicName) {
    if (this->topicRestricted && !this->isOperator(session)) {
        return NumericReply(ERR_CHANOPRIVSNEEDED) << session << this->name >> session;
    }
    this->setTopic(session, topicName);
}