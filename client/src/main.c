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

#define _TRUE 1
#define _FALSE 0
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
      fprintf(stderr,
	      "Not enough arguments.\n");
      fprintf(stderr,
	      _PATTERN);
      exit(EXIT_FAILURE);
    }
  else if (argc > 2)
    {
      fprintf(stderr,
	      "Too many arguments.\n");
      fprintf(stderr,
	      _PATTERN);
      exit(EXIT_FAILURE);
    }

  // connection vars
  int clientSocket;
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  socklen_t addr_size;

  // define client/server connection
  clientSocket = createStreamSocketWrapper();
  initSocketAddressWrapper(&serverAddress,
			   inet_addr(_SERVERIP),
			   _SERVERPORT);

  // connect to server
  connectWrapper(&clientSocket,
		 &serverAddress);
  
  if(strcmp(argv[1], "-m") == 0)
    {
      const char* message = "Hi server.";
      int returnVal;
      char response[256];
      int bytesRead;
      
      // attempt to send message to server      
      returnVal = send(clientSocket,
		       message,
		       strlen(message),
		       0);

      // wait for response
      bytesRead = recv(clientSocket,
		       response,
		       sizeof(response),
		       0);

      //print response
      response[bytesRead] = 0;
      fprintf(stdout,
	      "From server: %s\n",
	      response);
    }
  else if(strcmp(argv[1], "-f") == 0)
    {
      const char* fileName = "file.txt";
      char buffer[1];
      size_t bytesRead;
      FILE* inFile;
      int sendVal;

      // try opening file for reading
      inFile = fopen(fileName,
		     "r");
      if(inFile == NULL)
	{
	  perror("fopen() failed");
	  close(clientSocket);
	  exit(EXIT_FAILURE);
	}

      // send file data from open file to server byte by byte
      while(_TRUE)
	{
	  // read from file to buffer
	  bytesRead = fread(buffer,
			    1,
			    sizeof(buffer),
			    inFile);

	  // test that data wass successfully read
	  if(bytesRead == 0)
	    {
	      break;
	    }

	  // send read data to server
	  sendVal = send(clientSocket,
			 buffer,
			 bytesRead,
			 0);

	  // test for send success
	  if(sendVal == -1)
	    {
	      perror("send() failed");
	      break;
	    }
	}

      fclose(inFile);
    }
  
  // clean up
  close(clientSocket);
  
  return 0;
} // end of "main"
