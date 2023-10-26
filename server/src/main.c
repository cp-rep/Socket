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
  
  socketfd = createStreamSocketWrapper();

  struct sockaddr_in serverAddress;
  initSocketAddressWrapper(&serverAddress,
			   INADDR_ANY);

  bindWrapper(&socketfd, &serverAddress);

  listenWrapper(&socketfd, _QUEUELEN);

  

  return 0;
} // end of "main"
