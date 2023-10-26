#include "sockets.h"



/*
  Function:
   createStreamSocket

  Description:
   Attempts to open a socket of type SOCK_STREAM for TCP communication. If the socket
   fails to create, the program returns to the OS with an error code. Else, the 
   socket descriptor is returned to the caller.

  Input:
   None

  Output:
   int                  - An integer type containing the socket descriptor.
 */
int createStreamSocket()
{
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if(socketfd == -1)
    {
      fprintf(stderr, "Error creating socket communication type SOCK_STREAM\n");
      exit(EXIT_FAILURE);
    }

  return socketfd;
} // end of "createStreamSocket"



/*
  Function:
   initSocketAddress

  Description:
   Defines an incoming sockaddr_in with IpV4 address related data. Network
   byte order is handled where needed.

  Input:
   serverAddress        - A pointer to a sockaddr_int type that will be used
                          for defining IpV4 socket address data.

  Output:
   NONE
 */
void initSocketAddress(struct sockaddr_in* socketAddress, const in_addr_t addr)
{
  socketAddress->sin_family = AF_INET;
  socketAddress->sin_port = htons(_SERVERPORT);
  socketAddress->sin_addr.s_addr = addr;
} // end of "initServerAddress"
