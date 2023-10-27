/*
  File:
   main.c

  Description:
   The main driver file for the client side of client/server data transmission.
   The client attempts to connect with the server to send a buffer of data containing
   a message, and then awaits a response.  Or, the client attempts to send a file byte
   by byte to the server.
*/
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "sockets.h"

#define _SERVERIP "127.0.0.1"
#define _SERVERPORT 8000
#define _PATTERN "Pattern is: ./client [-m, -f] where 'm' is for message and 'f' for file\n"



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
  else if (argc > 2)
    {
      fprintf(stderr, "Too many arguments.\n");
      fprintf(stderr, _PATTERN);
      exit(EXIT_FAILURE);
    }

  // connection vars
  int clientSocket;
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  socklen_t addr_size;

  // define client/server connection
  clientSocket = createStreamSocketWrapper();
  initSocketAddressWrapper(&serverAddress, inet_addr(_SERVERIP), _SERVERPORT);

  // connect to server
  connectWrapper(&clientSocket, &serverAddress);
  
  if(strcmp(argv[1], "-m") == 0)
    {
      // attempt to send message to server
      const char* message = "from client";
      send(clientSocket, message, strlen(message), 0);

      // wait for response
      char response[256];
      int bytesRead = recv(clientSocket, response, sizeof(response), 0);

      //print response
      response[bytesRead] = 0;
      fprintf(stdout, "Server Response: %s\n", response);
    }
  else if(strcmp(argv[1], "-f") == 0)
    {
      
    }
  
  // clean up
  close(clientSocket);
  
  return 0;
} // end of "main"
