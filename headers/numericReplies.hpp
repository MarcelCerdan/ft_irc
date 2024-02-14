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
# define ERR_NICKNAMEINUSE(nick, new_nick) (":localhost 433 " + nick + " " + new_nick + " :Nickname is already in use\r\n")

// JOIN //
# define ERR_BADCHANMASK(channel) (channel + " :Bad channel name\r\n")

/*----- COMMANDS REPLIES -----*/

// JOIN //
# define JOINCHANNEL(nick, channel) (":" + nick + " JOIN " + channel + "\r\n")
# define RPL_TOPIC(nick, channel, topic) (":localhost 332 " + nick + " " + channel + " :" + topic + "\r\n")
# define RPL_NOTOPIC(nick, channel) (":localhost 331 " + nick + " " + channel + " :No topic is set\r\n")
# define RPL_NAMREPLY(nick, symbol, channel) (":localhost 353 " + nick + " " + symbol + channel + " :")
# define RPL_ENDOFNAMES(nick, channel) (":localhost 366 " + nick + " " + channel + " :End of /NAMES list\r\n")

/*----- REPLIES -----*/

# define RPL_WELCOME(nick) (":localhost 001 " + nick + " :Welcome to our IRC server " + nick + " !\r\n")
# define RPL_YOURHOST(nick) (": localhost 002 " + nick + " :Your host is localhost\r\n")
# define RPL_CREATED(nick, date) (":localhost 003 " + nick + " :This server was created : " + date + "\r\n")
# define RPL_MYINFO(nick) (":localhost 004 " + nick + " server name: localhost \nAvailable user modes: -Regular users\n\t-Operators\n \
							Available channel modes : -i: Set/remove Invite-only channel\n\t-t: Set/remove the restrictions of the TOPIC command to channel \
							operators\n\t-k: Set/remove the channel key (password)\n\t-o: Give/take channel operator privilege\n\t \
							-l: Set/remove the user limit to channel\r\n")

#endif