/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/28 03:44:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "main.hpp"

class Server;

void	pass(Server *serv, Message msg, int clientFd);
void	nick(Server *serv, Message msg, int clientFd);
void	user(Server *serv, Message msg, int clientFd);

#endif