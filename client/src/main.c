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
#define _HOSTIP "127.0.0.1"
#define _SERVERIP "127.0.0.1"
#define _SERVERPORT 8000
#define _CLIENTPACKETPORT 9000
#define _SERVERPACKETPORT 9000
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
  char response[256];
  size_t bytesRead;
  size_t bytesSent;

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
  bytesSent = send(clientSocket,
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
      const char* toServer = "Hi server.";

      // 5. attempt to send message to server
      bytesSent = sendWrapper(&clientSocket,
			      toServer,
			      strlen(toServer),
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
      FILE* inFile;
      const char etx[2] = { 3, '\0' };

      // try opening file for reading
      inFile = fopen(fileName,
		     "r");
      
      if(inFile == NULL)
	{
	  perror("fopen() failed");
	  close(clientSocket);
	  exit(EXIT_FAILURE);
	}

      // loop and send file data from the opened file to server
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
	      bytesSent = send(clientSocket,
			       etx,
			       strlen(etx),
			       0);

	      if(bytesSent == -1)
		{
		  perror("send() failed");
		  fclose(inFile);
		  close(clientSocket);
		  exit(EXIT_FAILURE);
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

	  // 5. attempt to send the read bytes from file to server
	  bytesSent = send(clientSocket,
			   buffer,
			   bytesRead,
			   0);

	  // test for send failure
	  if(bytesSent == -1)
	    {
	      perror("send() failed");
	      fclose(inFile);
	      close(clientSocket);
	      exit(EXIT_FAILURE);
	      break;
	    }
	}
      
      // close opened file
      fclose(inFile);
    }
  else if(strcmp(argv[1], "-p") == 0)
    {
      const char* toServer = "Sending packet.";
      
      // 5. attempt to send message to server
      bytesSent = sendWrapper(&clientSocket,
			      toServer,
			      strlen(toServer),
			      0);

      {
	int socketfdPacket;
	char packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
	struct sockaddr_in socketAddress;

	// sanity zero out the packet buffer
	for(int i = 0; i < sizeof(packet); i++)
	  {
	    packet[i] = 0;
	  }

	// create the socket
	socketfdPacket = createRawSocketTCP();
	
	// point the ip header to the start of the packet
	struct iphdr* ipHeader = (struct iphdr*)packet;

	// point the tcp header directly after the ip header since we're
	// sending an empty packet
	struct tcphdr* tcpHeader = (struct tcphdr*)(packet + sizeof(struct iphdr));

	// define the ip header
	defineIPHeader(ipHeader,
		       _HOSTIP,
		       _SERVERIP);

	// define the tcp header
	defineTCPHeader(tcpHeader,
			_CLIENTPACKETPORT,
			_SERVERPACKETPORT);

	// print ip/tcp headers to stdout before sending to server
	fprintf(stdout, "\n");
	fprintf(stdout, "Printing newly defined IP Header and TCP Header.\n");
	printIPHeaderClient(ipHeader);
	fprintf(stdout, "\n");
	printTCPHeaderClient(tcpHeader);
	fprintf(stdout, "\n");

	// define the server address
	initSocketAddressWrapper(&socketAddress,
				 ipHeader->daddr,
				 tcpHeader->dest);
	
	
	
	close(socketfdPacket);
      }
    }
  
  // clean up
  close(clientSocket);
  
  return 0;
} // end of "main"
