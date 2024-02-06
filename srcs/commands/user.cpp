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
	Client	*client = findClient(serv, clientFd);

	if (!client->getGoodPass())
		addToClientBuf(serv, clientFd, ERR_INPUTPASS);

	else if (client->getIsRegistered())
		addToClientBuf(serv, clientFd, ERR_ALREADYREGISTERED(client->getNickname()));

	else if (msg.getParams().size() < 4)
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client->getNickname(), msg.getCmd()));

	else if (msg.getParams()[1] != "0" || msg.getParams()[2] != "*")
		addToClientBuf(serv, clientFd, "The username must be followed by '0 *' \r\n");

	else
	{
		client->setUsername(msg.getParams()[0]);
		client->setRealName(msg.getParams()[3]);
		if (!client->getNickname().empty())
			client->setIsRegister();
	}
}
