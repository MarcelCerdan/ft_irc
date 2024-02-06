/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 19:10:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/27 19:10:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "main.hpp"

void pass(Server *serv, Message msg, int clientFd)
{
	Client *client = findClient(serv, clientFd);

	if (client->getGoodPass())
		addToClientBuf(serv, clientFd, ERR_ALREADYREGISTERED(client->getNickname()));

	else if (msg.getParams().empty())
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client->getNickname(), msg.getCmd()));

	else if (msg.getParams()[0] != serv->getPass())
	{
		addToClientBuf(serv, clientFd, ERR_PASSWDMISMATCH(client->getNickname()));
		//close connection with client
	}
	else
		client->setGoodPass();
}
