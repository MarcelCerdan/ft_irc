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

//TODO fix space pb in parsing when everything after a space is considered an argument. An arg can be separated by multiple spaces.
//TODO quit command

#include "main.hpp"

int Server::parseMsg(int clientFd, Message &msg) {

	std::map<const int, Client>::iterator it = _clients.find(clientFd);

	msg.splitMsg("\n");

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

	std::cout << std::endl << "NICK : " << _clients.find(clientFd)->second.getNickname() << std::endl;
	std::cout << "USER : " << _clients.find(clientFd)->second.getUsername() << std::endl;
	std::cout << "REAL : " << _clients.find(clientFd)->second.getRealName() << std::endl;
	std::cout << "Is registered : " << _clients.find(clientFd)->second.getIsRegistered() << std::endl;
	return (0);
}

void Server::registerClient(Message &msg, int clientFd) {

	msg.checkCmd();
	if (msg.getCmd() == "PASS")
		pass(this, msg, clientFd);
	else if (msg.getCmd() == "NICK")
		nick(this, msg, clientFd);
	else if (msg.getCmd() == "USER")
		user(this, msg, clientFd);

}

int	Message::parseCmd(int cmdNmb) {

	std::string	params;

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

	if (cpy.find_first_of(' ') != std::string::npos)
	{
		params.insert(0, cpy, cpy.find_first_of(' ') + 1);
		if (params.find('\r') != std::string::npos)
			params.erase(params.find('\r'), 1);
		splitParams(&params);
	}

	for (size_t i = 0; _cmd[i]; i++)
		_cmd[i] = static_cast<char>(std::toupper(_cmd[i]));

	/*std::cout << PURPLE << "PREFIX : " << _prefix << std::endl
				<< "CMD : " << _cmd << std::endl;
	if (!_params.empty())
	{
		std::cout << "PARAMS : ";
		for (std::size_t i = 0; i < _params.size(); i++)
			std::cout << _params[i] << " | ";
		std::cout << std::endl;
	}
	std::cout << "PARAMS : " << _params.size() << std::endl;
	std::cout << RESET << std::endl;*/

	return (0);
}

void	Message::checkCmd()
{
	if (_cmd.find("NICK") != std::string::npos && _cmd.find("PASS") != std::string::npos)
	{
		std::cout << "COMMAND : " << _cmd << std::endl;
		_cmd = "NICK";
		_params.erase(_params.begin());
	}
	else if (_cmd.find("USER") != std::string::npos && _cmd.find("NICK") != std::string::npos)
	{
		_cmd = "USER";
		_params.erase(_params.begin());
	}
}
