/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 22:32:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/25 22:32:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Client &findClient(Server *server, const int fd)
{
	std::map<const int, Client> &clients_list = server->getClients();
	std::map<const int, Client>::iterator it = clients_list.find(fd);

	return (it->second);
}

void	addToClientBuf(Server *serv, int const clientFd, std::string str)
{
	Client &client = findClient(serv, clientFd);

	client.setSendBuff(str);
}

int	sendMsg(int const clientFd, std::string &buf)
{
	int ret = send(clientFd, buf.c_str(), buf.size(), MSG_NOSIGNAL);

	if (ret < 0)
		perror("[Server] sendMsg");
	return (ret);
}

void	delClient(std::vector<pollfd> *pfds, std::vector<pollfd>::iterator it)
{
	close(it->fd);
	pfds->erase(it);
}
