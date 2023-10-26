/*
  File:
   main.c

  Description:

*/
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "sockets.h"


/*
  Function:
   main

  Description:
   The main driver function for the server side of client/server data transmission.
 */
int main(void)
{
  int socketfd = 0;
  
  socketfd = createStreamSocket();

  struct sockaddr_in serverAddress;
  initSocketAddress(&serverAddress,
		    INADDR_ANY);

  bindSocket(&socketfd, &serverAddress);

  

  return 0;
} // end of "main"
