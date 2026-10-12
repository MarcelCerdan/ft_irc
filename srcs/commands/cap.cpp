#include "main.hpp"

// No capabilities are supported: answer LS/LIST with an empty list and refuse
// every REQ, so that clients which negotiate (irssi, HexChat...) can register.
void cap(Server *serv, Message msg, int clientFd)
{
	Client &client = findClient(serv, clientFd);
	std::string nick = client.getNickname().empty() ? "*" : client.getNickname();

	if (msg.getParams().empty())
	{
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(nick, msg.getCmd()));
		return ;
	}

	std::string subCmd = msg.getParams()[0];
	if (subCmd == "LS" || subCmd == "LIST")
		addToClientBuf(serv, clientFd, RPL_CAP(nick, subCmd, ""));
	else if (subCmd == "REQ" && msg.getParams().size() > 1)
		addToClientBuf(serv, clientFd, RPL_CAP(nick, "NAK", msg.getParams()[1]));
}
