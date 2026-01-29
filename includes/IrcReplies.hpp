// ============= replies.hpp =============
#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

// ============= Base Components =============
#define PREFIX ":IRCServer "
#define POSTFIX "\r\n"

// ============= ERROR REPLIES (4xx) =============

// 401 ERR_NOSUCHNICK
#define ERR_NOSUCHNICK(nick) \
    (std::string(PREFIX) + "401 " + (nick) + " :No such nick/channel" + POSTFIX)

// 403 ERR_NOSUCHCHANNEL
#define ERR_NOSUCHCHANNEL(channel) \
    (std::string(PREFIX) + "403 " + (channel) + " :No such channel" + POSTFIX)

// 412 ERR_NOTEXTTOSEND
#define ERR_NOTEXTTOSEND(nick) \
    (std::string(PREFIX) + "412 " + (nick) + " :No text to send" + POSTFIX)

// 421 ERR_UNKNOWNCOMMAND
#define ERR_UNKNOWNCOMMAND(nick, command) \
    (std::string(PREFIX) + "421 " + (nick) + " " + (command) + " :Unknown command" + POSTFIX)

// 431 ERR_NONICKNAMEGIVEN
#define ERR_NONICKNAMEGIVEN() \
    (std::string(PREFIX) + "431 :No nickname given" + POSTFIX)

// 432 ERR_ERRONEUSNICKNAME
#define ERR_ERRONEUSNICKNAME(nick) \
    (std::string(PREFIX) + "432 " + (nick) + " :Erroneous nickname" + POSTFIX)

// 433 ERR_NICKNAMEINUSE
#define ERR_NICKNAMEINUSE(nick) \
    (std::string(PREFIX) + "433 " + (nick) + " :Nickname is already in use" + POSTFIX)

// 441 ERR_USERNOTINCHANNEL
#define ERR_USERNOTINCHANNEL(nick, user, channel) \
    (std::string(PREFIX) + "441 " + (nick) + " " + (user) + " " + (channel) + " :They aren't on that channel" + POSTFIX)

// 442 ERR_NOTONCHANNEL
#define ERR_NOTONCHANNEL(nick, channel) \
    (std::string(PREFIX) + "442 " + (nick) + " " + (channel) + " :You're not on that channel" + POSTFIX)

// 451 ERR_NOTREGISTERED
#define ERR_NOTREGISTERED(nick) \
    (std::string(PREFIX) + "451 " + (nick) + " :You have not registered" + POSTFIX)

// 461 ERR_NEEDMOREPARAMS
#define ERR_NEEDMOREPARAMS(nick, command) \
    (std::string(PREFIX) + "461 " + (nick) + " " + (command) + " :Not enough parameters" + POSTFIX)

// 462 ERR_ALREADYREGISTRED
#define ERR_ALREADYREGISTRED(nick) \
    (std::string(PREFIX) + "462 " + (nick) + " :You may not reregister" + POSTFIX)

// 464 ERR_PASSWDMISMATCH
#define ERR_PASSWDMISMATCH(nick) \
    (std::string(PREFIX) + "464 " + (nick) + " :Password incorrect" + POSTFIX)

// 471 ERR_CHANNELISFULL
#define ERR_CHANNELISFULL(nick, channel) \
    (std::string(PREFIX) + "471 " + (nick) + " " + (channel) + " :Cannot join channel (+l)" + POSTFIX)

// 473 ERR_INVITEONLYCHAN
#define ERR_INVITEONLYCHAN(nick, channel) \
    (std::string(PREFIX) + "473 " + (nick) + " " + (channel) + " :Cannot join channel (+i)" + POSTFIX)

// 475 ERR_BADCHANNELKEY
#define ERR_BADCHANNELKEY(nick, channel) \
    (std::string(PREFIX) + "475 " + (nick) + " " + (channel) + " :Cannot join channel (+k)" + POSTFIX)

// 482 ERR_CHANOPRIVSNEEDED
#define ERR_CHANOPRIVSNEEDED(nick, channel) \
    (std::string(PREFIX) + "482 " + (nick) + " " + (channel) + " :You're not channel operator" + POSTFIX)

// Mode-specific errors
#define ERR_NEEDMODEPARM(nick, channel, mode) \
    (std::string(PREFIX) + "696 " + (nick) + " " + (channel) + " " + (mode) + " :You must specify a parameter for the mode" + POSTFIX)

#define ERR_INVALIDMODEPARM(nick, channel, mode) \
    (std::string(PREFIX) + "696 " + (nick) + " " + (channel) + " " + (mode) + " :Invalid mode parameter" + POSTFIX)

#define ERR_UNKNOWNMODE(nick, channel, mode) \
    (std::string(PREFIX) + "472 " + (nick) + " " + (channel) + " " + (mode) + " :is not a recognised channel mode" + POSTFIX)

// ============= SUCCESS REPLIES (0xx-3xx) =============

// 001 RPL_WELCOME
#define RPL_WELCOME(nick, user, host) \
    (std::string(PREFIX) + "001 " + (nick) + " :Welcome to the Internet Relay Network " + (nick) + "!" + (user) + "@" + (host) + POSTFIX)

// 002 RPL_YOURHOST
#define RPL_YOURHOST(nick) \
    (std::string(PREFIX) + "002 " + (nick) + " :Your host is IRCServer, running version 1.0" + POSTFIX)

// 003 RPL_CREATED
#define RPL_CREATED(nick) \
    (std::string(PREFIX) + "003 " + (nick) + " :This server was created today" + POSTFIX)

// 004 RPL_MYINFO
#define RPL_MYINFO(nick) \
    (std::string(PREFIX) + "004 " + (nick) + " IRCServer 1.0 io iklot" + POSTFIX)

// 331 RPL_NOTOPIC
#define RPL_NOTOPIC(nick, channel) \
    (std::string(PREFIX) + "331 " + (nick) + " " + (channel) + " :No topic is set" + POSTFIX)

// 332 RPL_TOPIC
#define RPL_TOPIC(nick, channel, topic) \
    (std::string(PREFIX) + "332 " + (nick) + " " + (channel) + " :" + (topic) + POSTFIX)

// 341 RPL_INVITING
#define RPL_INVITING(nick, target, channel) \
    (std::string(PREFIX) + "341 " + (nick) + " " + (target) + " " + (channel) + POSTFIX)

// 353 RPL_NAMREPLY
#define RPL_NAMREPLY(nick, channel, users) \
    (std::string(PREFIX) + "353 " + (nick) + " = " + (channel) + " :" + (users) + POSTFIX)

// 366 RPL_ENDOFNAMES
#define RPL_ENDOFNAMES(nick, channel) \
    (std::string(PREFIX) + "366 " + (nick) + " " + (channel) + " :End of /NAMES list" + POSTFIX)

// ============= COMMAND REPLIES (No numeric code) =============

// JOIN
#define RPL_JOIN(prefix, channel) \
    (std::string(":") + (prefix) + " JOIN :" + (channel) + POSTFIX)

// PART
#define RPL_PART(prefix, channel, reason) \
    (std::string(":") + (prefix) + " PART " + (channel) + " :" + (reason) + POSTFIX)

// NICK
#define RPL_NICK(prefix, newnick) \
    (std::string(":") + (prefix) + " NICK :" + (newnick) + POSTFIX)

// QUIT
#define RPL_QUIT(prefix, reason) \
    (std::string(":") + (prefix) + " QUIT :Quit: " + (reason) + POSTFIX)

// KICK
#define RPL_KICK(prefix, channel, target, reason) \
    (std::string(":") + (prefix) + " KICK " + (channel) + " " + (target) + " :" + (reason) + POSTFIX)

// MODE
#define RPL_MODE(prefix, channel, modes, params) \
    (std::string(":") + (prefix) + " MODE " + (channel) + " " + (modes) + " " + (params) + POSTFIX)

// INVITE
#define RPL_INVITE(prefix, target, channel) \
    (std::string(":") + (prefix) + " INVITE " + (target) + " :" + (channel) + POSTFIX)

// PRIVMSG
#define RPL_PRIVMSG(prefix, target, msg) \
    (std::string(":") + (prefix) + " PRIVMSG " + (target) + " :" + (msg) + POSTFIX)

// NOTICE
#define RPL_NOTICE(prefix, target, msg) \
    (std::string(":") + (prefix) + " NOTICE " + (target) + " :" + (msg) + POSTFIX)

// TOPIC (command, not numeric)
#define RPL_TOPICCHANGE(prefix, channel, topic) \
    (std::string(":") + (prefix) + " TOPIC " + (channel) + " :" + (topic) + POSTFIX)

// PONG
#define RPL_PONG(server, token) \
    (std::string(":") + (server) + " PONG " + (server) + " :" + (token) + POSTFIX)

#endif // REPLIES_HPP