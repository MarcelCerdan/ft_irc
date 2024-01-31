#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

# include "main.hpp"

class Client {

private:
	const int	_socket;
	std::string _readBuff;
	std::string _nickname;
	std::string	_username;
	std::string _realName;
	bool		_goodPass;
	bool		_isRegistered;

public:
	Client(const int socket);
	Client(const Client &other);
	~Client();

	Client &operator=(const Client &other);

	void	setReadBuff(std::string const &msg);
	void	setNickname(std::string const &newNickname);
	void	setUsername(std::string const &username);
	void	setRealName(std::string const &realName);
	void	setGoodPass();

	std::string &getReadBuff();
	std::string	&getNickname();
	std::string	&getUsername();
	bool		getGoodPass();
	bool		getIsRegistered();
	int 		getSocket();

};


#endif //FT_IRC_CLIENT_HPP
