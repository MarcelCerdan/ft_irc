/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:20:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/02/16 15:20:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "main.hpp"

void invite(Server *serv, Message msg, int clientFd) {
	if (!checkClient(serv, msg, clientFd) || !checkChannel(serv, msg, clientFd))
		return ;

	//Client &client = findClient(serv, clientFd);
	//Client &invitedClient = find()
}