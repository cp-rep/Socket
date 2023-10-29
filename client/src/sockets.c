/*
  File:
   sockets.c

  Description:
   The implementations of socket.h functions.
*/
#include "sockets.h"



/*
  Function:
   createStreamSocketWrapper

  Description:
   Attempts to open a socket of type SOCK_STREAM for TCP communication. If the socket
   fails to create, the program returns to the OS with an error code. Else, the 
   socket descriptor is returned to the caller.

  Input:
   None

  Output:
   int                  - An integer type containing the socket descriptor.
 */
int createStreamSocketWrapper()
{
  int socketfd = socket(AF_INET,
			SOCK_STREAM,
			0);

  if(socketfd == -1)
    {
      perror("socket() failed");
      exit(EXIT_FAILURE);
    }

  return socketfd;
} // end of "createStreamSocketWrapper"



/*
  Function:
   initSocketAddressWrapper

  Description:
   Defines an incoming sockaddr_in with IPv4 address related data. Network
   byte order is handled where needed.

  Input:
   serverAddress        - A pointer to a sockaddr_int type that will be used
                          for defining IPv4 socket address data.

  Output:
   NONE
 */
void initSocketAddressWrapper(struct sockaddr_in* socketAddress,
			      const in_addr_t addr,
			      const int port)
{
  socketAddress->sin_family = AF_INET;
  socketAddress->sin_addr.s_addr = addr;
  socketAddress->sin_port = htons(port);
} // end of "initSocketAddressWrapper"



/*
  Function:
   bindWrapper

  Description:
   Binds the incoming socket to the incoming address data.  If it fails,
   the program exits returning an error code to the OS.

  Input:
   socketfd             - A pointer to an integer holding the address of the
                          socket file descriptor to bind.

   sockaddr_int         - A pointer to a sockaddr_in structure that contains
                          address information to bind to bind with the 
                          incoming socket descriptor.
  Output:
   None
*/
int bindWrapper(int* socketfd,
		 struct sockaddr_in* socketAddress)
{
  int bindVal;
  bindVal = bind(*socketfd,
		 (struct sockaddr*)&(*socketAddress),
		 sizeof(*socketAddress));

  if(bindVal == -1)
    {
      perror("bind() failed");
      close(*socketfd);
      exit(EXIT_FAILURE);
    }

  return bindVal;
} // end of "bindWrapper"



/*
  Function:
   listenWrapper

  Description:
   Attempts to listen on the incoming socket descriptor for incoming
   connections.
   
  Input:
   socketfd             - a pointer to an int type containing the file descriptor
                          of a socket.

   queueLen             - a const int type that represents the total amount of 
                          connections can be made at once before turning them down.

  Output:
   NONE
*/
int listenWrapper(int* socketfd,
		   const int queueLen)
{
  int listenVal;
  listenVal = listen(*socketfd,
		     queueLen);

  if(listenVal == -1)
    {
      perror("listen() failed");
      close(*socketfd);
      exit(EXIT_FAILURE);
    }

  return listenVal;
} // end of "listenWrapper"



/*
  Function:
   connectWrapper

  Description:
   Attempts to connect the client socket to a provided server address. If failure 
   to connect, an error is printed and program is exited returning the error to the OS.
   
  Input:
   clientSocket         - A pointer to an integer containing the client socket file
                          descriptor.

   serverAddress        - A pointer to a struct sockaddr_in type that contains defined
                          IPv4 server address data.

  Output:
   int                  - Zero on connection success.
 */
int connectWrapper(int* clientSocket,
		    struct sockaddr_in* serverAddress)
{
  int connectValue;
  
  connectValue = connect(*clientSocket,
			 (struct sockaddr*)*(&serverAddress),
			 sizeof(*serverAddress));

  if(connectValue == -1)
    {
      perror("connect() failed");
      close(*clientSocket);
      exit(EXIT_FAILURE);
    }

  return connectValue;
} // end of "connectWrapper"



/*
  Function:
   acceptWrapper

  Description:
   The server attempts to accept a connection with the client from the provided 
   client address.  If the connection is successful, the client socket descriptor 
   is stored and returned to the caller.

  Input:
   clientSocket         - A pointer to an integer of an undefined client socket file
                          descriptor.

   serverSocket         - A pointer to an integer of a server socket file descriptor
                          that describes an initialized server address of TCP type
                          with IPv4 address data.

  Output:
   clientSocket         - The newly defined client socket file descriptor.
*/
int acceptWrapper(int* clientSocket,
		   int* serverSocket,
		   struct sockaddr_in* clientAddress)
{
  socklen_t clientAddressLen = sizeof(*clientAddress);
  
  *clientSocket = accept(*serverSocket,
			 (struct sockaddr*)*(&clientAddress),
			 &clientAddressLen);

  if(*clientSocket == -1)
    {
      perror("Accept Failed");
      close(*serverSocket);
      exit(EXIT_FAILURE);
    }
  return *clientSocket;
} // end of "acceptWrapper"



/*
  Function:
   recvWrapperServer

  Description:
   Attempts to read data from the client storing it in the provided buffer.  If there
   is any error in the read, the error is handled with a cleanup, the error is prompted
   to stdout and the program exits with a failure code.
   
  Input:
   clientSocket         - A pointer to an integer representing  aserver socket file
                          descriptor.

   serverSocket         - A pointer to an integer representing a client socket file
                          descriptor.

   buff                 - A pointer to the beginning address of a character array
                          that will be used to store data received from the client.

   flag                 - The flag value to be used in the recv() call.

  Output:
   int                  - The number of total bytes read from the client.
*/
int recvWrapperServer(int* clientSocket,
		      int* serverSocket,
		      char* buff,
		      const int buffSize,
		      const int flag)
{
  int bytesRead;
  
  bytesRead = recv(*clientSocket,
		   buff,
		   buffSize,
		   flag);
  
  if(bytesRead  == -1)
    {
      perror("recv() failed");
      close(*clientSocket);
      close(*serverSocket);
      exit(EXIT_FAILURE);
    }

  return bytesRead;
} // end of "recvWrapperServer"






/*
  Function:
   recvWrapperClient

  Description:
   Attempts to read data from the server storing it in the provided buffer.  If there
   is any error in the read, the error is handled with a cleanup, the error is prompted
   to stdout and the program exits with a failure code.
   
  Input:
   clientSocket         - A pointer to an integer representing  aserver socket file
                          descriptor.

   buff                 - A pointer to the beginning address of a character array
                          that will be used to store data received from the server.

   flag                 - The flag value to be used in the recv() call.

  Output:
   int                  - The number of total bytes read from the server.
*/
int recvWrapperClient(int* clientSocket,
		      char* buff,
		      const int buffSize,
		      const int flag)
{
  int bytesRead;
  
  bytesRead = recv(*clientSocket,
		   buff,
		   buffSize,
		   flag);
  
  if(bytesRead  == -1)
    {
      perror("recv() failed");
      close(*clientSocket);
      exit(EXIT_FAILURE);
    }

  return bytesRead;
} // end of "recvWrapperClient"



/*
  Function:
   fopenServerWrapper

  Description:
   Attempts to open a file for writing, in this case on the server.  If there is an
   error opening the file, a cleanup is triggered closing the open socket file 
   descriptors, an error is prompted to stdout,  and the program exits with 
   an error code.

  Input:
   clientSocket         - A pointer to an integer representing a server socket file
                          descriptor.

   serverSocket         - A pointer to an integer representing a client socket file
                          descriptor.

   saveFile             - a pointer to a pointer to a FILE type that will be used
                          to store the address of an opened file.

   fileName             - A pointer to a char containing the filename/path to open.

   mode                 - A pointer to a char containing the mode in which the file
                          should be opened.

  Output:
   FILE*                - A pointer to a FILE type containing the file
                          descriptor of the open file.
*/
FILE* fopenServerWrapper(int* clientSocket,
			 int* serverSocket,
			 FILE** saveFile,
			 char* fileName,
			 char* mode)
{
  *saveFile = fopen(fileName,
		    mode);
  
  if(*saveFile == NULL)
    {
      perror("fopen() failed");
      close(*clientSocket);
      close(*serverSocket);
      exit(EXIT_FAILURE);
    }

  return *saveFile;
} // end of "fopenServerWrapper"
