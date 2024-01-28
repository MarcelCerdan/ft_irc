#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

# include "main.hpp"

class Client {

private:
	const int	_socket;
	std::string _readBuff;
	std::string _nickname;
	std::string	_username;
	bool		_goodPass;
	bool		_isRegistered;

public:
	Client(const int socket);
	Client(const Client &other);
	~Client();

	Client &operator=(const Client &other);

	void	setReadBuff(std::string const &msg);
	void	setNickname(std::string const &newNickname);
	void	setGoodPass();

	std::string &getReadBuff();
	std::string	&getNickname();
	std::string	&getUsername();
	bool		getGoodPass();
	bool		getIsRegistered();

};


#endif //FT_IRC_CLIENT_HPP
