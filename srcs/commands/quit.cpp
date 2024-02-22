#include "main.hpp"

void quit(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);
	std::string preMessage = client.getNickname() + " QUIT :";
	std::string reason = "";

	client.setIsConnected(false);
	if (msg.getParams().size() > 0)
		reason = msg.getParams()[0];
	addToClientBuf(serv, clientFd, preMessage + reason + "\r\n");
}
