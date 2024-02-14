#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

enum e_modes {
	i = 0,
	t = 1,
	k = 2,
	l = 3
};

class Channel {
private:
	std::string						_name;
	std::string						_topic;
	std::vector<Client *>			_members;
	std::map<const int, Client &>	_chanOps;
	bool							_modes[3];

public:
	Channel(std::string &name, Server *serv, int clientFd);
	Channel(const Channel &other);
	~Channel();
	Channel &operator=(const Channel &other);

	std::string	&getTopic();
	std::string getName() const;
	std::vector<Client *>	&getMembers();
	std::map<const int, Client &> &getChanOps();
	const bool*	getModes() const;

	void	setTopic(std::string &newTopic);
	void	setMode(int i, int sign);
	void	addMember(Client *newMember);
	void	addChanOps(int clientFd, Client &newChanOps);
};

//std::map<const std::string, Channel>::iterator findChannel(Server *serv, std::string const &name);

#endif
