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
	Client *client = &serv->getClients().find(clientFd)->second;

	//if (client.getGoodPass())
	//	send(clientFd, ERR_ALREADYREGISTERED(client.getNickname()))

	if (msg.getParams().empty())
		std::cout << ERR_NEEDMOREPARAMS(client->getNickname(), msg.getCmd()) << std::endl;

	else if (msg.getParams()[0] != serv->getPass())
	{
		std::cout << ERR_PASSWDMISMATCH(client->getNickname()) << std::endl;
		//close connection with client;
	}
	else
		client->setGoodPass();
}