/*
  File:
   main.c

  Description:
   The main driver file for the server side of client/server data transmission.
   The server accepts incoming connections to read a buffer of data containing
   a message or for receiving a file from the client connection.
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

  int serverSocket;
  int clientSocket;
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLen;

  // create the socket
  serverSocket = createStreamSocketWrapper();

  // define server address IPv4 data
  initSocketAddressWrapper(&serverAddress,
			   INADDR_ANY,
			   _SERVERPORT);
  
  // bind the socket to server address
  bindWrapper(&serverSocket,
	      &serverAddress);

  // set socket into listening mode
  listenWrapper(&serverSocket,
		    _QUEUELEN);

  // prompt stdout that the server is now listening
  fprintf(stdout, "Servering listening on port %d...\n", _SERVERPORT);

  // accept and handle incoming connections
  acceptWrapper(&clientSocket,
		&serverSocket,
		&clientAddress);
  
  if(strcmp(argv[1], "-m") == 0)
    {
      const char* toClient = "You have successfully connected to the server.";
      int sendVal;
      char buff[256];
      int bytesRead;

      // attempt to send message to client      
      sendVal = send(clientSocket,
		       toClient,
		       strlen(toClient),
		       0);
      
      // attempt to receive message from client
      bytesRead = recvWrapper(&clientSocket,
			      &serverSocket,
			      buff,
			      sizeof(buff),
			      0);

      // null terminate the received message
      buff[bytesRead] = 0;

      // print received message to stdout
      fprintf(stdout, "From client:  %s\n", buff);
    }
  else if(strcmp(argv[1], "-f") == 0)
    {
      char buff[1];
      FILE* saveFile;
      int bytesRead;
      char* fileName = "receivedFile.txt";
      char* mode = "w+";
      int writeVal;

      // open file for writing
      fopenServerWrapper(&clientSocket,
			 &serverSocket,
			 &saveFile,
			 fileName,
			 mode);

      // loop and receive data from client
      while(_TRUE)
	{
	  // receive data
	  bytesRead = recv(clientSocket,
			   buff,
			   sizeof(buff),
			   0);

	  // check for error
	  if(bytesRead < 0)
	    {
	      perror("recv() failed");
	      break;
	    }
	  // test for EOF
	  else if(bytesRead == 0)
	    {
	      break;
	    }

	  // write data to file
	  writeVal = fwrite(buff,
			    1,
			    bytesRead,
			    saveFile);
	  
	  // test for bad write
	  if(writeVal < 0)
	    {
	      perror("fwrite() failed");
	      break;
	    }
	}

      // clean up
      fclose(saveFile);
      close(clientSocket);
      close(serverSocket);
    }
  
  return 0;
} // end of "main"
