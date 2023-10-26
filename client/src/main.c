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
#include "sockets.h"



/*
  Function:
   main

  Description:
   The main driver function for the client side of client/server data transmission.
*/
int main(void)
{

  int clientSocket;
  struct sockaddr_in serverAddress;

  // create client socket
  clientSocket = createStreamSocketWrapper();

  // define the IPv4 server address
  initSocketAddressWrapper(&serverAddress,
			   inet_addr("127.0.0.1")); // loopback

  // attempt connection to server
  connectWrapper(&clientSocket, &serverAddress);

  const char* message = "from client";
  send(clientSocket, message, strlen(message), 0);

  char response[256];
  int bytesRead = recv(clientSocket, response, sizeof(response), 0);

  response[bytesRead] = 0;
  fprintf(stdout, "Server Response: %s\n", response);
  
  return 0;
} // end of "main"
