//
// Created by 이석규 on 2024. 8. 21..
//

#include "ChatService.hpp"

#define RPL_QUIT_REASON(nickname, reason) (":" + nickname + " QUIT : " + reason + CRLF)

void ChatService::quit(Session &session, const Message &message) {
    std::string params = message.getParam();

    // TODO: Channel에 Client 저장되어 있는 방식 확인
    //  session에서 Client 정보를 가져와 Channel들의 해당 Client 삭제
    //  Channel의 다른 Client 에게 Quit-message 전송
    //  만약 Channel에 속한 인원이 0명이면 Channel 삭제

    (void) session;
}