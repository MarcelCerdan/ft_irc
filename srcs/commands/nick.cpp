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
	Client &client = serv->getClients().find(clientFd)->second;
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
		std::cout << ERR_NONICKNAMEGIVEN(client.getNickname()) << std::endl;

	else if (params[0].find_first_of("#:") == 0)
		std::cout << ERR_ERRONEUSNICKNAME(client.getNickname(), params[0]) << std::endl;

	else
		client.setNickname(params[0]);
}
