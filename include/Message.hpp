//
// Created by 김세진 on 8/13/24.
//

#ifndef FT_IRC_MESSAGE_HPP
#define FT_IRC_MESSAGE_HPP

#include <sstream>
#include <vector>

class Message {
public:
    typedef enum command_e {
        UNKNOWN,
        // connection message
        PASS,
        NICK,
        USER,
        PING,
        QUIT,
        // channel operation
        JOIN,
        PART,
        TOPIC,
    } command_t;

    Message();
    Message(command_t command, std::vector<std::string> const &params);
    ~Message();
    Message &operator=(Message const &other);

    size_t getParamSize() const;
    command_t getCommand() const;
    std::string getParam(size_t index) const;
    std::string getParam() const;
    std::string getParamsAll(int index, char ignore = 0) const;
    std::vector<std::string> getSplitedParam(int index, char delimiter) const;

private:
    command_t command;
    std::vector<std::string> params;
};

#endif //FT_IRC_MESSAGE_HPP
