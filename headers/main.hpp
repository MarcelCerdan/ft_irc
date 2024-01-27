#ifndef MAIN_HPP
# define MAIN_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <cstring>
# include <string>
# include <cstdlib>
# include <cstdio>
# include <cerrno>
# include <vector>
# include <map>
# include <arpa/inet.h>
# include <poll.h>
# include <unistd.h>
# include <csignal>

# include "errorMessages.hpp"
# include "colors.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Message.hpp"

# define MAX_CLIENT 10
# define BUFF_SIZE	512

#endif
