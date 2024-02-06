/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 07:44:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/28 07:44:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "main.hpp"

void nick(Server *serv, Message msg, int clientFd)
{
	Client *client = findClient(serv, clientFd);
	std::vector<std::string> params = msg.getParams();

	if (!client->getGoodPass())
		addToClientBuf(serv, clientFd, "Error : please input the password first");

	else if (params.empty())
		addToClientBuf(serv, clientFd, ERR_NONICKNAMEGIVEN(client->getNickname()));

	else if (params[0].find_first_of("#:") == 0)
		addToClientBuf(serv, clientFd, ERR_ERRONEUSNICKNAME(client->getNickname(), params[0]));

	else
	{
		client->setNickname(params[0]);
		if (!client->getUsername().empty())
			client->setIsRegister();
	}

}
