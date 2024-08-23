//
// Created by 김세진 on 8/21/24.
//

#include "Channel.hpp"

#include <NumericReply.hpp>

#define RPL_JOIN_CHANNEL(nickname, channelName) (nickname + " JOIN " + channelName)

Channel::Channel(std::string const &name) : name(name) {}

Channel::~Channel() {}

void Channel::join(Session *session, const std::string &key) {
    (void) key;
    // TODO: Channel에 session이 있는지 확인 (hasSession)
    //  MODE 설정 되어있는지 확인 (각 MODE에 따라 예외)

    participants.insert(session);

    std::string userList;
    // TODO: Operator 인지 아닌지 구분하기
    //  Channel 이 UserList를 들고 display

    // TODO: 나중에 방의 옵션(#, &)을 달아줘야 함
    this->broadcast(RPL_JOIN_CHANNEL(session->getNickname(), name));
    if (!topic.empty()) {
        *session << NumericReply::channelReply(RPL_TOPIC, session->getNickname(), name, topic);
    }
    *session << NumericReply::channelReply(RPL_NAMREPLY, session->getNickname(), name, userList);
    *session << NumericReply::channelReply(RPL_ENDOFNAMES, session->getNickname(), name, std::string("End of /NAMES list"));
}

int Channel::remove(Session *session) {
    session->leaveChannel(name);
    operators.erase(session);
    participants.erase(session);
    return participants.size();
}

void Channel::operator<<(const std::string &message) {
    broadcast(message);
}

void Channel::broadcast(std::string const &message) {
    for (Sessions::iterator it = participants.begin(); it != participants.end(); it++) {
        (*it)->operator<<(message);
    }
}

bool Channel::isOperator(Session *session) const {
    return operators.find(session) != operators.end();
}

bool Channel::isParticipant(Session *session) const {
    return participants.find(session) != participants.end();
}

Session *Channel::getParticipant(std::string const &nickname) const {
    for (Sessions::iterator it = participants.begin(); it != participants.end(); it++) {
        if ((*it)->getNickname() == nickname) {
            return *it;
        }
    }
    return NULL;
}

void Channel::setTopic(std::string const &topicName) {
    // TODO: t 옵션일 시 operator 인지 확인하기

    this->topic = topicName;
    NumericReply(RPL_TOPIC, this->topic) >> this;
}

void Channel::displayTopic(Session *session) {
    if (topic.empty()) {
        return NumericReply(RPL_NOTOPIC) >> session;
    }
    return NumericReply(RPL_TOPIC, this->topic) >> session;
}
