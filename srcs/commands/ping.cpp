#include "main.hpp"

void ping(Server *serv, Message msg, int clientFd)
{
	Client &client = findClient(serv, clientFd);

	if (msg.getParams().empty())
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
	else
		addToClientBuf(serv, clientFd, PONG(msg.getParams()[0]));
}
