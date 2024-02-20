#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

enum e_modes {
	e_i = 0,
	e_t = 1,
	e_k = 2,
	e_l = 3
};

class Channel {
private:
	char 							_creationDate[100];
	std::string						_name;
	std::string						_topic;
	std::string						_password;
	std::vector<std::string>		_invites;
	std::vector<Client *>			_members;
	std::map<const int, Client &>	_chanOps;
	bool							_modes[3];
	int								_maxUsers;

public:
	Channel(std::string &name, Server *serv, int clientFd);
	Channel(const Channel &other);
	~Channel();
	Channel &operator=(const Channel &other);

	std::string	&getTopic();
	std::string getName() const;
	std::string getPassword() const;
	std::vector<std::string>		getInvites() const;
	std::vector<Client *>			&getMembers();
	std::map<const int, Client &>	&getChanOps();
	const bool*						getModes() const;
	int 							getMaxUsers() const;
	char*							getCreationDate();

	void	setTopic(std::string &newTopic);
	void	setPassword(std::string newPassword);
	void	setMode(int i, int sign);
	void	setMaxUsers(int newMaxUsers);

	void	addInvite(std::string &nick);
	void	addMember(Client *newMember);
	void	addChanOps(int clientFd, Client &newChanOps);

	void	eraseMember(Client &client);
};

//std::map<const std::string, Channel>::iterator findChannel(Server *serv, std::string const &name);
void printChannelInfo(Channel &channel);
bool isOperator(Client &client, Channel &channel);
std::string intToString(int number);

#endif
