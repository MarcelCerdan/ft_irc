/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:50:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/30 15:50:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "main.hpp"

void user(Server *serv, Message msg, int clientFd)
{
	Client	&client = serv->getClients().find(clientFd)->second;

	if (client.getIsRegistered())
		std::cout << ERR_ALREADYREGISTERED(client.getNickname()) << std::endl;

	else if (msg.getParams().size() < 4)
		std::cout << ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()) << std::endl;

	else if (msg.getParams()[1] != "0" || msg.getParams()[2] != "*")
		std::cout << "The username must be followed by '0 *' /r/n" << std::endl;

	else
	{
		client.setUsername(msg.getParams()[0]);
		client.setRealName(msg.getParams()[3]);
	}
}
