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
  int returnVal;
  char response[256];
  int bytesRead;

  // define client/server connection
  clientSocket = createStreamSocketWrapper();
  initSocketAddressWrapper(&serverAddress,
			   inet_addr(_SERVERIP),
			   _SERVERPORT);

  // 1. connect to server
  connectWrapper(&clientSocket,
		 &serverAddress);

  // 2. wait for response
  bytesRead = recvWrapperClient(&clientSocket,
				response,
				sizeof(response),
				0);

  response[bytesRead] = 0;
  fprintf(stdout,
	  "%s\n",
	  response);

  // 3. send command option to server
  returnVal = send(clientSocket,
		   argv[1],
		   strlen(argv[1]),
		   0);

  // 4. receive that the server read the sent option successfully
  bytesRead = recvWrapperClient(&clientSocket,
				response,
				sizeof(response),
				0);
  response[bytesRead] = 0;

  // confirm the client provided a correct command line option
  if(strcmp(response, "Failed to read option.") == 0)
    {
      close(clientSocket);
      fprintf(stderr, "Something went wrong with the option sent to server.\n");
      exit(EXIT_FAILURE);
    }
    else
      {
	fprintf(stdout,
		"%s\n",
		response);
      }
  
  if(strcmp(argv[1], "-m") == 0)
    {
      const char* message = "Hi server.";
      //const char* message = "q";      

      // 5. attempt to send message to server      
      returnVal = send(clientSocket,
		       message,
		       strlen(message),
		       0);

      // 6. wait for response
      bytesRead = recvWrapperClient(&clientSocket,
				    response,
				    sizeof(response),
				    0);

      //print response
      response[bytesRead] = 0;
      fprintf(stdout,
	      "Server: %s\n",
	      response);
    }
  else if(strcmp(argv[1], "-f") == 0)
    {
      const char* fileName = "file.txt";
      char buffer[1];
      size_t bytesRead;
      FILE* inFile;
      int sendVal;
      char etx[2] = { 3, '\0' };

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
	  // test that data was successfully read from file
	  if(bytesRead == 0)
	    {

	      // 6. attempt send ETX to server
	      returnVal = send(clientSocket,
			       etx,
			       strlen(etx),
			       0);

	      if(returnVal == -1)
		{
		  perror("send() failed");
		}
	      
	      // 7. attempt to receive success message
	      bytesRead = recvWrapperClient(&clientSocket,
					    response,
					    sizeof(response),
					    0);

	      response[bytesRead] = 0;
	      fprintf(stdout,
		      "Server: %s\n",
		      response);
	      break;
	    }

	  // 5. attempt to send data to server
	  sendVal = send(clientSocket,
			 buffer,
			 bytesRead,
			 0);

	  // test for send failure
	  if(sendVal == -1)
	    {
	      perror("send() failed");
	      break;
	      fclose(inFile);
	      close(clientSocket);
	      exit(EXIT_FAILURE);
	    }
	}
      
      // close opened file
      fclose(inFile);
    }

  // clean up
  close(clientSocket);
  
  return 0;
} // end of "main"
