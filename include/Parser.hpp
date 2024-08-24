//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_PARSER_HPP
#define FT_IRC_PARSER_HPP

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include "Message.hpp"

class Parser {
public:
    static int parsePort(char *port);
    static int parseLimit(std::string const &limit);
    static Message parseMessage(std::stringstream &stream);
    static Message::command_t parseCommand(std::string const &command);
};

#endif //FT_IRC_PARSER_HPP
