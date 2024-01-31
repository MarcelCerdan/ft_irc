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
# define ERR_ALREADYREGISTERED(nick) (":localhost 462 " + nick + " :You may not reregister\r\n")

// PASS //
# define ERR_PASSWDMISMATCH(nick) (":localhost 464 " + nick + " :Password incorrect\r\n")

// NICK //
# define ERR_NONICKNAMEGIVEN(nick) (":localhost 431 " + nick + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(nick, new_nick) (":localhost 432 " + nick + " " + new_nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick, new_nick) ("localhost 433 " + nick + " " + new_nick + " :Nickname is already in use\r\n")

#endif