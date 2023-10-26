#include "sockets.h"



/*
  Function:
   createStreamSocket

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
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if(socketfd == -1)
    {
      perror("Error creating socket communication type SOCK_STREAM.");
      exit(EXIT_FAILURE);
    }

  return socketfd;
} // end of "createStreamSocket"



/*
  Function:
   initSocketAddress

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
			      const in_addr_t addr)
{
  socketAddress->sin_family = AF_INET;
  socketAddress->sin_port = htons(_SERVERPORT);
  socketAddress->sin_addr.s_addr = addr;
} // end of "initServerAddress"



/*
  Function:
   bindSocket

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
void bindWrapper(int* socketfd,
		 struct sockaddr_in* socketAddress)
{
  int tempInt;
  tempInt = bind(*socketfd, (struct sockaddr*)&(*socketAddress), sizeof(*socketAddress));

  if(tempInt == -1)
    {
      perror("Error binding socket to address.\n");
      close(*socketfd);
      exit(EXIT_FAILURE);
    }
} // end of "bindSocket"



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
void listenWrapper(int* socketfd,
		   const int queueLen)
{
  int listenVal;
  listenVal = listen(*socketfd, queueLen);

  if(listenVal == -1)
    {
      perror("Error binding socket to address.\n");
      close(*socketfd);
      exit(EXIT_FAILURE);
    }
} // end of "listenWrapper"