/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/02/21 13:44:00 by mthibaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	COMMANDS_HPP
# define COMMANDS_HPP

# include "main.hpp"

class Server;

void	pass(Server *serv, Message msg, int clientFd);
void	nick(Server *serv, Message msg, int clientFd);
void	user(Server *serv, Message msg, int clientFd);
void	join(Server *serv, Message msg, int clientFd);
void	privmsg(Server *serv, Message msg, int clientFd);
void	mode(Server *serv, Message msg, int clientFd);
void	invite(Server *serv, Message msg, int clientFd);
void 	topic(Server *serv, Message msg, int clientFd);
void 	quit(Server *serv, Message msg, int clientFd);
void	kick(Server *serv, Message msg, int clientFd);
void	part(Server *serv, Message msg, int clientFd);

#endif