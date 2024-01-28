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
				if (msg.parseCmd(static_cast<int>(i)))
					registerClient(msg, clientFd);
			}
		}
	}
}

void Server::registerClient(Message &msg, int clientFd) {

	std::map<const int, Client>::iterator it = _clients.find(clientFd);

	if (msg.getCmd() == "PASS")
		pass(this, msg, clientFd);
	else if (msg.getCmd() == "NICK")

}

int	Message::parseCmd(int cmdNmb) {

	if (_splitMsg[cmdNmb].empty())
		return (-1);

	std::string	cpy = _splitMsg[cmdNmb];
	if (cpy[0] == ':') // If there's a prefix
	{
		if (cpy.find_first_of(' ') != std::string::npos)
		{
			_prefix.insert(0, cpy, 0, cpy.find_first_of(' '));
			cpy.erase(0, cpy.find_first_of(' ') + 1);
		}
	}

	if (cpy.find_first_of(' ') == std::string::npos) // if it's a command without params
	{
		_cmd = cpy;
		if (_cmd.find('\r') != std::string::npos)
			_cmd.erase(_cmd.find('\r'), 1);
	}
	else
		_cmd.insert(0, cpy, 0, cpy.find_first_of(' '));

	_param.insert(0, cpy, cpy.find_first_of(' ') + 1);
	_param.erase(_param.find('\r'), 1);

	for (size_t i = 0; _cmd[i]; i++)
		_cmd[i] = static_cast<char>(std::toupper(_cmd[i]));

	std::cout << PURPLE << "PREFIX : " << _prefix << std::endl
				<< "CMD : " << _cmd << std::endl
				<< "PARAMs : " << _param << std::endl;

	return (0);
}