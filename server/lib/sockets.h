/*
  File:
   sockets.h

  Description:
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

#define _SERVERPORT 8000

int createStreamSocket();
void initSocketAddress(struct sockaddr_in* socketAddress, const in_addr_t addr);


#endif
