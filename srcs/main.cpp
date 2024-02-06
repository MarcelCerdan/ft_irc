#include "classes/Server.hpp"

bool	checkArgs(int ac, char **av);

void	signalHandler(int signal)
{
	if (signal == SIGINT) // shutdown the server
		exit (0);
	else if (signal == SIGTSTP)
		std::cout << "Received SIGTSTP (CTRL+Z)" << std::endl;
}

int	main(int ac, char **av)
{
	if (!checkArgs(ac, av))
		return (0);

	std::string password = av[2];

	signal(SIGINT, signalHandler);
	signal(SIGTSTP, signalHandler);

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