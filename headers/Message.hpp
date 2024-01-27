/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:14:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/27 00:14:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "main.hpp"

class Message {

private:
	std::string	const				_fullMsg;
	std::vector<std::string> 		_splitMsg;
	std::string 					_prefix;
	std::string						_cmd;
	std::string 					_param;

public:
	Message(std::string msg);
	Message(const Message &other);
	~Message(void);

	Message &operator=(const Message &other);

	std::string	const &getFullMsg();
	std::vector<std::string> &getSplitMsg();
	std::string	&getPrefix();
	std::string	&getCmd();
	std::string &getParam();

	void	splitMsg(std::string const &delimiter);
};

#endif
