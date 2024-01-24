#ifndef ERRORMESSAGES_HPP
# define ERRORMESSAGES_HPP

# define ERR_PORT_VALUE "The port must be a valid number between 1025 and 65535"
# define ERR_ARG_NMB "Wrong number of arguments, to run ircserv type :\n./ircsrv <port> <password>"
# define ERR_GETADDINFO "Error in getaddrinfo : "
# define ERR_SOCKET "Error in the socket creation"
# define ERR_START_SERV "Error when starting the server"
# define ERR_POLL "Error : poll() failed"
# define ERR_ACCEPT "Error : accept() failed"
# define ERR_FULL_SERV "Error : the server is already full !"

#endif
