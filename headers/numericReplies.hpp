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

// COMMANDS //
# define ERR_NEEDMOREPARAMS(nick, command) (":localhost 461 " + nick + command + " :Not enough parameters\r\n")

// PASS //
# define ERR_ALREADYREGISTERED(nick) (":localhost 462 " + nick + " :You may not reregister\r\n")
# define ERR_PASSWDMISMATCH(nick) (":localhost 464 " + nick + " :Password incorrect\r\n")

#endif