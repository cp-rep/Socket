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



/*
  Function:
   main
*/
int main(int argc, char** argv)
{
  int serverSocket;
  int clientSocket;
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLen;
  int sendVal;
  char buff[256];
  int bytesRead;


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
  fprintf(stdout,
	  "Servering listening on port %d...\n",
	  _SERVERPORT);

  // 1. accept and handle incoming connections
  acceptWrapper(&clientSocket,
		&serverSocket,
		&clientAddress);

  const char* toClient = "You have successfully connected to the server.";

  // 2. attempt to let the client know the connection was successful
  sendVal = send(clientSocket,
		 toClient,
		 strlen(toClient),
		 0);
  
  // 3. receive the clients command option
  bytesRead = recvWrapperServer(&clientSocket,
				&serverSocket,
				buff,
				sizeof(buff),
				0);

  buff[bytesRead] = 0;

  // test which option the client chose
  if(strcmp(buff, "-m") == 0)
    {
      const char* toClient = "The server will now attempt to read your sent messages.";

      // 4. attempt to send message
      sendVal = send(clientSocket,
		     toClient,
		     strlen(toClient),
		     0);

      // 5. attempt to receive message
      bytesRead = recvWrapperServer(&clientSocket,
				    &serverSocket,
				    buff,
				    sizeof(buff),
				    0);

      // null terminate the received message
      buff[bytesRead] = 0;

      // print received message to stdout
      fprintf(stdout,
	      "Client:  %s\n",
	      buff);
	  
      const char* toClient2 = "Hello client";
      // 6. send message to client
      sendVal = send(clientSocket,
		     toClient2,
		     strlen(toClient2),
		     0);
	  
    }
  else if(strcmp(buff, "-f") == 0)
    {
      char buff[1];
      FILE* saveFile;
      int bytesRead;
      char* fileName = "receivedFile.txt";
      char* mode = "w+";
      int writeVal;

      const char* toClient = "The server will now attempt to receive your file.";

      // 4. attempt to send message
      sendVal = send(clientSocket,
		     toClient,
		     strlen(toClient),
		     0);
      
      // open file for writing
      fopenServerWrapper(&clientSocket,
			 &serverSocket,
			 &saveFile,
			 fileName,
			 mode);

      // loop and receive data
      while(_TRUE)
	{
	  // 5. or 6. - attempt receive data
	  bytesRead = recvWrapperServer(&clientSocket,
					&serverSocket,
					buff,
					sizeof(buff),
					0);
	  buff[bytesRead] = 0;

	  // test for ETX from client
	  if(buff[0] == 3)
	    {
	      const char* toClient3 = "File received successfully.";
	      
	      // 7. attempt to send success message
	      sendVal = send(clientSocket,
			     toClient3,
			     strlen(toClient3),
			     0);	      
	      break;
	    }

	  // safety test for EOF 
	  if(bytesRead == 0)
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

      // close opened file
      fclose(saveFile);
    }
  else
    {
      const char* optionFailed = "Failed to read option.";

      // 4. attempt to send message
      sendVal = send(clientSocket,
		     optionFailed,
		     strlen(optionFailed),
		     0);      
      fprintf(stderr, "Something went wrong with the option received from client.\n");
    }

  // clean up
  close(clientSocket);
  close(serverSocket);
  
  return 0;
} // end of "main"
