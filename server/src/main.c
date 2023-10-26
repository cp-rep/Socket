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


#define _TRUE 1
#define _FALSE 0

/*
  Function:
   main

  Description:
   The main driver function for the server side of client/server data transmission.
*/
int main(void)
{
  int serverSocket = 0;
  struct sockaddr_in serverAddress;
  
  // creat the socket
  serverSocket = createStreamSocketWrapper();

  // define server address IPv4 data
  initSocketAddressWrapper(&serverAddress,
			   INADDR_ANY);
  
  // bind the socket to server address
  bindWrapper(&serverSocket, &serverAddress);
  
  // set socket into listening mode
  listenWrapper(&serverSocket,
		_QUEUELEN);

  fprintf(stdout, "Servering listening on port %d...\n", _SERVERPORT);
  
  while(_TRUE)
    {
      // handle incoming connections
      struct sockaddr_in clientAddress;
      socklen_t clientAddressLen = sizeof(clientAddress);
      
      int clientSocket = accept(serverSocket,
				(struct sockaddr*)&clientAddress,
				&clientAddressLen);
      if(clientSocket == -1)
	{
	  perror("Error accepting connection.\n");
	  continue;
	}

      const char* response = "Hello.";
      send(clientSocket, response, strlen(response), 0);
    }
  
  return 0;
} // end of "main"
