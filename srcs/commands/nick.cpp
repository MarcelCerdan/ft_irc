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

//TODO client nickname can't have space ' ', comma ',', asterisk '*', question mark '?', exclamation mark '!', at sign '@', dot '.'
//TODO client nickname can't start with '#', '&', '~', '+'

static bool	nickInUse(Server *serv, std::string &nick);
static bool isValidNickname(const std::string& nick);

void nick(Server *serv, Message msg, int clientFd)
{
	Client &client = findClient(serv, clientFd);
	std::vector<std::string> params = msg.getParams();

	if (!client.getGoodPass())
		addToClientBuf(serv, clientFd, "Error : please input the password first\r\n");
	else if (params.empty())
		addToClientBuf(serv, clientFd, ERR_NONICKNAMEGIVEN(client.getNickname()));
	else if (!isValidNickname(params[0]))
		addToClientBuf(serv, clientFd, ERR_ERRONEUSNICKNAME(client.getNickname(), params[0]));

	else if (nickInUse(serv, params[0]))
		addToClientBuf(serv, clientFd, ERR_NICKNAMEINUSE(client.getNickname(), params[0]));
	else
	{
		client.setNickname(params[0]);
		if (!client.getUsername().empty())
		{
			client.setIsRegister();
			printServInfo(serv, clientFd);
		}
	}

}

static bool isValidNickname(const std::string& nick) {
    if (nick.empty())
        return (false);
    if (nick[0] == ':' || nick[0] == '#' || nick[0] == '&' || nick[0] == '~' || nick[0] == '+')
        return (false);
    if (nick.find_first_of(" ,*?!@.") != std::string::npos)
        return (false);
    return (true);
}

static bool	nickInUse(Server *serv, std::string &nick)
{
	std::map<const int, Client>::iterator it;

	for (it = serv->getClients().begin(); it != serv->getClients().end(); it++)
	{
		if (it->second.getNickname() == nick)
			return (true);
	}

	return (false);
}
