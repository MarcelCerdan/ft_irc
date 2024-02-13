#include "main.hpp"
#include <sstream>

/*
privmsg send privates messages between users and channels.
target is the nickname of a client or the name of a channel.
if the user is in a channel and is banned, the message is not delivered.
if can't be delivered, the server respond with ERR_CANNOTSENDTOCHAN (404) numeric
to let the user know.
If target = channel name -> prefix with one or more channel membership prefix character(@, +, etc).
The message will be delivered only to the members of the channel with the given or higher status in the channel.


check NOTICE command after



so:
-parse privmsg commands -> extract the target and the message.
The target can be a channel name or a user nickname.
-if the target is a user, send the message to that user.
-if the target is a channel, check for channel modes and user permissions.
Handle bans, exceptions, and other channel-specific conditions.
If the tharget is a user who is away, send an RPL_AWAY response.
-handle errors
-if target start with dollar character broadcast message to all clients on one or multiple servers.


for sending client just addtoclientbuf with clientfd of target, if list split and for loop
for channels, for loop members.getSocket

how to find clientfd of the target:
-I have to parse the first param to see if it's a list, a client or a channel.
I make two vectors, one client and one channel and stock during the parsing.

if # at the beginning, it's a channel



  :Angel PRIVMSG Wiz :Hello are you receiving this message ?
                                  ; Message from Angel to Wiz.

  :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
                                  ; Message from dan to the channel
                                  #coolpeople
*/

void privmsg(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);
	std::string targets = msg.getParams()[0];
	std::vector<std::string> stringChannels;
	std::vector<std::string> stringClients;
	std::string	message;
	std::map<const int, Client> &clients = serv->getClients();
	std::map<const std::string, Channel> channels = serv->getChannels();

	if (msg.getParams().size() < 2) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}
	message = msg.getParams()[1];

	std::istringstream iss(targets);
	std::string target;
	while (std::getline(iss, target, ',')) {
		if (!target.empty() && target[0] == '#')
			stringChannels.push_back(target);
		else
			stringClients.push_back(target);
	}

	for (std::vector<std::string>::iterator it1 = stringClients.begin(); it1 != stringClients.end(); it1++) {
		std::string targetClientName = *it1;
		for (std::map<const int, Client>::iterator it2 = clients.begin(); it2 != clients.end(); it2++) {
			if (targetClientName == it2->second.getNickname()) {
				addToClientBuf(serv, it2->second.getSocket(), message + "\r\n");
			}
		}
	}
}
