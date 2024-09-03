//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_SESSION_HPP
#define FT_IRC_SESSION_HPP

#include <iostream>
#include <set>
#include "Socket.hpp"
#include "Message.hpp"
#include "Parser.hpp"

class Channel;

class Session : public Socket {
public:
    typedef std::set<Channel *> Channels;

    Session(Socket::fd_t fd, std::string const &address);
    ~Session();

    bool operator>>(Message &message);
    bool operator==(Session const &other);

    bool isPassed() const;
    bool isRegistered() const;
    std::string const &getNickname() const;
    std::string const &getUsername() const;
    std::string const &getServername() const;
    std::string getAddress() const;

    void setPassed();
    void setRegistered();
    void updateNickname(std::string const &nickname);
    void updateUser(std::string const &username, std::string const &hostname, std::string const &servername,
                    std::string const &realname);

    Channels getJoinedChannels() const;
    void leaveChannel(Channel *channel);
    void joinChannel(Channel *channel);

private:
    const std::string address;
    bool passed;
    bool registered;
    std::string nickname;
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
    Channels channels;
};

#endif //FT_IRC_SESSION_HPP
