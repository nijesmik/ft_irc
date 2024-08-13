//
// Created by 김세진 on 8/12/24.
//

#ifndef FT_IRC_PARSER_HPP
#define FT_IRC_PARSER_HPP

#include <cstdlib>
#include <stdexcept>
#include "Message.hpp"

class Parser {
public:
    static int parsePort(char *port);
    static Message parseMessage(std::string const &raw);
};

#endif //FT_IRC_PARSER_HPP
