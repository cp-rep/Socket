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
#define _PACKETPORT 8080
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
  char buff[256];
  size_t bytesSent;
  size_t bytesRead;

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

  const char* connectMsg = "You have successfully connected to the server.";

  // 2. attempt to let the client know the connection was successful
  bytesSent = sendWrapper(&clientSocket,
			  connectMsg,
			  strlen(connectMsg),
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
      const char* readMsg = "The server will now attempt to read your sent messages.";
      
      // 4. attempt to send message
      bytesSent = sendWrapper(&clientSocket,
			      readMsg,
			      strlen(readMsg),
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

      const char* toClientMsg = "Hello client.";
      
      // 6. send message to client
      bytesSent = sendWrapper(&clientSocket,
			      toClientMsg,
			      strlen(toClientMsg),
			      0);
    }
  else if(strcmp(buff, "-f") == 0)
    {
      char buff[1];
      FILE* saveFile;
      const char* fileName = "receivedFile.txt";
      const char* mode = "w+";
      int bytesWritten;

      const char* fileMsg = "The server will now attempt to receive your file.";

      // 4. attempt to send message
      bytesSent = sendWrapper(&clientSocket,
			      fileMsg,
			      strlen(fileMsg),
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
	      const char* fileReceivedMsg = "File received successfully.";
	      
	      // 7. attempt to send success message
	      bytesSent = send(clientSocket,
			       fileReceivedMsg,
			       strlen(fileReceivedMsg),
			       0);	      
	      break;
	    }

	  // safety test for EOF 
	  if(bytesRead == 0)
	    {
	      break;
	    }

	  // write data to file
	  bytesWritten = fwrite(buff,
				1,
				bytesRead,
				saveFile);

	  // test for bad write
	  if(bytesWritten < 0)
	    {
	      perror("fwrite() failed");
	      break;
	    }
	}

      // close opened file
      fclose(saveFile);
    }
  else if(strcmp(buff, "-p") == 0)
    {
      const char* fileMsg = "The server will now attempt to receive a packet.";

      // 4. attempt to send message
      bytesSent = sendWrapper(&clientSocket,
			      fileMsg,
			      strlen(fileMsg),
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

      {
	int socketfdPacket;
	struct sockaddr socketAddress;
	const int socketAddrSize = sizeof(socketAddress);
	char packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];

	// sanity zero out the packet buffer
	for(int i = 0; i < sizeof(packet); i++)
	  {
	    packet[i] = 0;
	  }
	
	// open new raw socket for TCP packet transfer
	socketfdPacket = createRawSocketTCP();

	// receive packet
	bytesReceived = recvfrom(socketfdPacket,
				 packet,
				 sizeof(packet),
				 0,
				 &socketAddress,
				 (socklen_t*)&socketAddrSize);
	    
	// check if bytes were received from the client
	if(bytesReceived > 0)
	  {
	    fprintf(stdout, "Packet received successfully.\n");	    
	  }
	else
	  {
	    perror("recvfrom() failed");
	    break;
	  }

	close(socketfdPacket);
      }


      
    }
  else
    {
      const char* optionFailed = "Failed to read option.";

      // 4. attempt to send message
      bytesSent = send(clientSocket,
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
