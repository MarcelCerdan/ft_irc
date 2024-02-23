#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

# include "main.hpp"

class Client {
private:
	const int	_socket;
	std::string _readBuff;
	std::string _sendBuff;
	std::string _nickname;
	std::string	_username;
	std::string _realName;
	bool		_goodPass;
	bool		_isRegistered;
	bool		_isConnected;
	std::vector<std::string> _channels;

public:
	Client(int socket);
	Client(const Client &other);
	~Client();

	Client &operator=(const Client &other);

	void	setReadBuff(std::string const &msg);
	void	setSendBuff(std::string const &msg);
	void	setNickname(std::string const &newNickname);
	void	setUsername(std::string const &username);
	void	setRealName(std::string const &realName);
	void	setGoodPass();
	void	setIsRegister();
	void	setIsConnected(bool newState);

	std::string &getReadBuff();
	std::string &getSendBuff();
	std::string	&getNickname();
	std::string	&getUsername();
	std::string &getRealName();
	std::vector<std::string> &getChannels();
	bool		&getGoodPass();
	bool		&getIsRegistered();
	bool		getIsConnected() const;
	int 		getSocket();

	void		freeSendBuffer();
	void		freeReadBuffer();
	void		addChannel(std::string const &channel);
	void		removeChannel(std::string const &channel);

};

int	sendMsg(int clientFd, std::string &buf);

#endif
