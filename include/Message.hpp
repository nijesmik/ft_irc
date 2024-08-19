//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_MESSAGE_HPP
#define FT_IRC_MESSAGE_HPP

#include <string>
#include <vector>

class Message {
public:
    typedef enum command_e {
        UNKNOWN,
        PASS,
        NICK,
    } command_t;

    Message();
    Message(command_t command, std::vector<std::string> const &params);
    ~Message();
    Message &operator=(Message const &other);

    command_t getCommand() const;
    std::string getParam(size_t index) const;
    std::string getParam() const;

private:
    command_t command;
    std::vector<std::string> params;
};

#endif //FT_IRC_MESSAGE_HPP
