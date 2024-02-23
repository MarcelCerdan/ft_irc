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

	msg.splitMsg();

	for (std::size_t i = 0; i != msg.getSplitMsg().size(); i++)
	{
		if (msg.parseCmd(static_cast<int>(i)) == 0)
		{
			if (!it->second.getIsRegistered())
				registerClient(msg, clientFd);
			else
			{
				for(std::map<const std::string, cmdFunction>::iterator it = _cmdList.begin(); it != _cmdList.end(); it++)
				{
					if (it->first == msg.getCmd())
						it->second(this, msg, clientFd);
				}
			}
		}
	}

	return (0);
}

void Server::registerClient(Message &msg, int clientFd) {

	if (msg.getCmd() == "PASS")
		pass(this, msg, clientFd);
	if (msg.getCmd() == "NICK")
		nick(this, msg, clientFd);
	if (msg.getCmd() == "USER")
		user(this, msg, clientFd);

}

int	Message::parseCmd(int cmdNmb) {

	std::string	params;

	if (_splitMsg[cmdNmb].empty())
		return (-1);

	std::string	cpy = _splitMsg[cmdNmb];
	if (cpy[0] == ':') // If there's a prefix
	{
		_prefix.erase();
		size_t prefixEnd = cpy.find_first_of(' ');
		if (prefixEnd != std::string::npos)
		{
			_prefix.insert(0, cpy, 0, prefixEnd);
			cpy.erase(0, prefixEnd + 1);
		}
	}

	size_t cmdEnd = cpy.find_first_of(' ');
	_cmd.erase();
	if (cmdEnd == std::string::npos) // if it's a command without params
	{
		_cmd = cpy;
		if (_cmd.find('\r') != std::string::npos)
			_cmd.erase(_cmd.find('\r'), 1);
	}
	else
		_cmd.insert(0, cpy, 0, cpy.find_first_of(' '));

	std::cout << _cmd << std::endl;

	size_t paramsStart = cpy.find_first_not_of(' ', cmdEnd);
	if (paramsStart != std::string::npos) {
		params.insert(0, cpy, paramsStart);
		if (params.find('\r') != std::string::npos)
			params.erase(params.find('\r'), 1);
		splitParams(&params);
	}

	for (size_t i = 0; _cmd[i]; i++)
		_cmd[i] = static_cast<char>(std::toupper(_cmd[i]));
	return (0);
}

