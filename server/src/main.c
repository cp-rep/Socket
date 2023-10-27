/*
  File:
   main.c

  Description:
   The main driver file for the server side of client/server data transmission.
   The server accepts incoming connections to read buffer of data containing
   a message, or receiving a file from the client connection.
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
#define _SERVERIP "127.0.0.1"
#define _SERVERPORT 8000
#define _QUEUELEN 10
#define _PATTERN "Pattern is: ./server [-m, -f] where 'm' is for message and 'f' for file\n"



/*
  Function:
   main
*/
int main(int argc, char** argv)
{
  if(argc == 1)
    {
      fprintf(stderr, "Not enough arguments.\n");
      fprintf(stderr, _PATTERN);
      exit(EXIT_FAILURE);
    }
  else if(argc > 2)
    {
      fprintf(stderr, "Too many arguments.\n");
      fprintf(stderr, _PATTERN);
      exit(EXIT_FAILURE);
    }

  if(strcmp(argv[1], "-m") == 0)
    {
  
      int serverSocket = 0;
      struct sockaddr_in serverAddress;
  
      // creat the socket
      serverSocket = createStreamSocketWrapper();

      // define server address IPv4 data
      initSocketAddressWrapper(&serverAddress,
			       INADDR_ANY,
			       _SERVERPORT);
  
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
    }  
  else if(strcmp(argv[1], "-f") == 0)
    {

    }
  
  return 0;
} // end of "main"
