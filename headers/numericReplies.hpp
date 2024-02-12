/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericReplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:12:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/28 03:12:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

/*----- COMMANDS ERRORS -----*/

# define ERR_NEEDMOREPARAMS(nick, command) (":localhost 461 " + nick + " " + command + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTERED(nick) (":localhost 462 " + nick + " :You may not reregiste\r\n")

// PASS //
# define ERR_PASSWDMISMATCH(nick) (":localhost 464 " + nick + " :Password incorrect\r\n")

// NICK //
# define ERR_NONICKNAMEGIVEN(nick) (":localhost 431 " + nick + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(nick, new_nick) (":localhost 432 " + nick + " " + new_nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick, new_nick) ("localhost 433 " + nick + " " + new_nick + " :Nickname is already in use\r\n")

/*----- COMMANDS REPLIES -----*/

// JOIN //
# define JOINCHANNEL(nick, channel) (":" + nick + " JOIN " + channel + "\r\n")
# define RPL_TOPIC(nick, channel, topic) (":localhost 332 " + nick + " " + channel + " :" + topic + "\r\n")
# define RPL_NOTOPIC(nick, channel) (":localhost 331 " + nick + " " + channel + " :No topic is set\r\n")
# define RPL_NAMREPLY(nick, symbol, channel) (":localhost 353 " + nick + " " + symbol + channel + " :")
# define RPL_ENDOFNAMES(nick, channel) (":localhost 366 " + nick + " " + channel + " :End of /NAMES list\r\n")

// PRIVMSG //
# define ERR_CANNOTSENDTOCHAIN(nick, channel) (":localhost 404 " + nick + " " + channel + " :Cannot send to channel\r\n")
# define ERR_NOSUCHNICK(nick) ("localhost 401 " + nick + ": No such nick/channel\r\n")
# define ERR_NOSUCHSERVER(nick) ("localhost 402 " + nick + " " + "server name" + " :No such server\r\n")
# define ERR_TOOMANYTARGETS(target) ("localhost 407 " + target + ":Duplicate recipients. No message delivered")
# define ERR_NORECIPIENT(nick) ("localhost 411 " + nick + " :No recipient given (" + "'command'" + ")\r\n")
# define ERR_NOTEXTTOSEND(nick) ("localhost 412 " + nick + " :No text to send")

#endif