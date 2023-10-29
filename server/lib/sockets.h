/*
  File:
   sockets.h

  Description:
   Contains the function declartions for socket related functions defined
   in the sockets.c file.
*/
#ifndef SOCKETS_H
#define SOCKETS_H
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int createStreamSocketWrapper();
void initSocketAddressWrapper(struct sockaddr_in* socketAddress,
			      const in_addr_t addr,
			      const int port);
int bindWrapper(int* socketfd,
		 struct sockaddr_in* address);
int listenWrapper(int* socketfd,
		   const int queueLen);
int connectWrapper(int* clientSocket,
		    struct sockaddr_in* serverAddress);
int acceptWrapper(int* clientSocket,
		   int* serverSocket,
		   struct sockaddr_in* clientAddress);
size_t recvWrapperServer(int* clientSocket,
		      int* serverSocket,
		      char* responseBuff,
		      const int buffSize,
		      const int flag);
FILE* fopenServerWrapper(int* clientSocket,
			 int* serverSocket,
			 FILE** saveFile,
			 char* fileName,
			 const char* mode);
size_t recvWrapperClient(int* clientSocket,
		      char* buff,
		      const int buffSize,
		      const int flag);
size_t sendWrapper(int* clientSocket,
		const char* buff,		   
		const int buffSize,
		const int flag);
#endif
