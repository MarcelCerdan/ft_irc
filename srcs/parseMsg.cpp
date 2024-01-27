/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseMsg.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 22:47:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/26 22:47:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int Server::parseMsg(int clientFd, Message &msg) {

	std::map<const int, Client>::iterator it = _clients.find(clientFd);

	msg.splitMsg("\n");

	for (std::size_t i = 0; i <= msg.getSplitMsg().size(); i++)
	{
		if (!it->second.getGoodPass())
		{
			if (!it->second.getIsRegistered())
			{
				registerClient(msg.getSplitMsg()[i], clientFd);
			}
		}
	}
}

void Server::registerClient(std::string cmd, int clientFd) {

	
}
