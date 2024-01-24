#include "Server.hpp"

bool	checkArgs(int ac, char **av);

int	main(int ac, char **av)
{
	if (!checkArgs(ac, av))
		return (0);

	std::string password = av[2];

	Server server(av[1], password);
	server.start();

}

bool	checkArgs(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << RED << ERR_ARG_NMB << RESET << std::endl;
		return (false);
	}

	for (int i = 0; av[1][i]; i++)
	{
		if (!isdigit(av[1][i]))
		{
			std::cout << RED << ERR_PORT_VALUE << RESET << std::endl;
			return (false);
		}
	}

	if (atoi(av[1]) < 1025 || atoi(av[1]) > 65535)
	{
		std::cout << RED << ERR_PORT_VALUE << RESET << std::endl;
		return (false);
	}

	return (true);
}
