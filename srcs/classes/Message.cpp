/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:14:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/27 00:14:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "classes/Message.hpp"

Message::Message(std::string msg) : _fullMsg(msg) {}

Message::Message(const Message &other) {

	*this = other;
}

Message::~Message(void) {}

Message &Message::operator=(const Message &other) {

	if (this != &other) {
		_cmd = other._cmd;
		_params = other._params;
		_prefix = other._prefix;
	}
	return (*this);
}

std::string const &Message::getFullMsg() { return (_fullMsg); }

std::vector<std::string> &Message::getSplitMsg() { return (_splitMsg); }

std::string &Message::getPrefix() { return (_prefix); }

std::string &Message::getCmd() { return (_cmd); }

std::vector<std::string> &Message::getParams() { return (_params); }

void Message::splitMsg(std::string const &delimiter) {

	std::size_t	pos;
	std::string	substr;

	while ((pos = _fullMsg.find(delimiter)) != std::string::npos)
	{
		substr = _fullMsg.substr(0, pos);
		_splitMsg.push_back(substr);
		_fullMsg.erase(0, pos + delimiter.length());
	}

}

void Message::splitParams(std::string *params) {

	std::size_t posSpace;
	std::size_t posColon = params->find(':');
	std::string substr;

	while ((posSpace = params->find(' ')) != std::string::npos)
	{
		if (posColon != std::string::npos && posSpace > posColon)
		{
			substr = params->substr(posColon);
			_params.push_back(substr);
			break;
		}
		substr = params->substr(0, posSpace);
		_params.push_back(substr);
		params->erase(0, posSpace + 1);
	}
	_params.push_back(*params);
}
