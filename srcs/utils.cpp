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

void	printServInfo(Server *serv, int clientFd)
{
	Client &client = findClient(serv, clientFd);
	std::string date = serv->getCreationDate();

	addToClientBuf(serv, clientFd, RPL_WELCOME(client.getNickname()));
	addToClientBuf(serv, clientFd, RPL_YOURHOST(client.getNickname()));
	addToClientBuf(serv, clientFd, RPL_CREATED(client.getNickname(), date));
	addToClientBuf(serv, clientFd, RPL_MYINFO(client.getNickname()));
}

bool	checkClient(Server *serv, Message msg, int clientFd) {
	std::map<const int, Client> clientsList = serv->getClients();

	for (std::map<const int, Client>::iterator it = clientsList.begin(); it != clientsList.end(); it++) {
		if (it->second.getNickname() == msg.getParams()[0])
			return (true);
	}

	addToClientBuf(serv, clientFd, ERR_NOSUCHNICK(msg.getParams()[0]));
	return (false);
}

bool	checkChannel(Server *serv, Message msg, int clientFd) {
	std::map<const std::string, Channel> channelsList = serv->getChannels();

	for (std::map<const std::string, Channel>::iterator it = channelsList.begin(); it != channelsList.end(); it++) {
		if (it->second.getName() == msg.getParams()[1])
			return (true);
	}

	addToClientBuf(serv, clientFd, ERR_NOSUCHCHANNEL(findClient(serv, clientFd).getNickname(), msg.getParams()[1]));
	return (false);
}

void printChannelInfo(Channel &channel) {
    std::cout << "Channel Members: ";
    const std::vector<Client *> &members = channel.getMembers();
    for (size_t i = 0; i < members.size(); ++i) {
        std::cout << members[i]->getNickname() << " ";
    }
    std::cout << std::endl;

    std::cout << "Channel ChanOps: ";
    const std::map<const int, Client &> &chanOps = channel.getChanOps();
    for (std::map<const int, Client &>::const_iterator it = chanOps.begin(); it != chanOps.end(); ++it) {
        std::cout << it->second.getNickname() << " ";
    }
    std::cout << std::endl;
}