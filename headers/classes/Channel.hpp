#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

class Channel {
private:
	std::string	const				_name;
	std::string						_topic;
	std::vector<Client *>			_members;
	std::map<const int, Client *>	_chanOps;

public:
	Channel(std::string &name, Server *serv, const int clientFd);
	Channel(const Channel &other);
	~Channel();
	Channel &operator=(const Channel &other);

	std::string	&getTopic();
	std::string getName();
	std::vector<Client *>	&getMembers();
	std::map<const int, Client *> &getChanOps();

	void	setTopic(std::string &newTopic);
	void	addMember(Client *newMember);
	void	addChanOps(const int clientFd, Client *newChanOps);
};

Channel	*findChannel(Server *serv, std::string const &name);

#endif
