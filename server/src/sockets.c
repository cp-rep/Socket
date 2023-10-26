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
