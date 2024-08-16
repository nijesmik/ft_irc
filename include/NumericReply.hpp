//
// Created by 김세진 on 8/16/24.
//

#ifndef FT_IRC_NUMERICREPLY_HPP
#define FT_IRC_NUMERICREPLY_HPP

#define DELIMITER " "
#define MESSAGE_PREFIX ":"
#define CRLF "\r\n"

#define ERR_UNKNOWNCOMMAND 421
#define ERR_UNKNOWNCOMMAND_MESSAGE "Unknown command"

#define ERR_NEEDMOREPARAMS 461
#define ERR_NEEDMOREPARAMS_MESSAGE "Not enough parameters"

#define ERR_ALREADYREGISTRED 462
#define ERR_ALREADYREGISTRED_MESSAGE "You may not reregister"

#define ERR_PASSWDMISMATCH 464
#define ERR_PASSWDMISMATCH_MESSAGE "Password incorrect"

#include <sstream>

class NumericReply {
public:
    static std::string get(int code);
    static std::string get(int code, std::string const &param);

private:
    static std::string message(int code);
    static void append(std::stringstream &ss, int num);
    static void append(std::stringstream &ss, std::string const &str);
    static void appendMessage(std::stringstream &ss, int code);
};

#endif //FT_IRC_NUMERICREPLY_HPP
