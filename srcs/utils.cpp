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

Client *findClient(Server *server, const int fd)
{
	std::map<const int, Client> clients_list = server->getClients();
	std::map<const int, Client>::iterator it = clients_list.find(fd);

	if (it == clients_list.end())
		return (NULL);
	return (&it->second);
}
