#include "main.hpp"

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

*/

void privmsg(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);

	std::cout << "client privmsg: " << client.getNickname() << std::endl;
	std::cout << "msg full msg privmsh: " << msg.getFullMsg() << std::endl;
}