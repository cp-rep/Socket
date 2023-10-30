# Socket

## What is Socket?
Socket is a basic lower level approach to synchronizing data transmission between a client and server.  It allows the client to send a message, file, or packet to the server.  The client and server are both currently set to the loopback address for communication.

## How to Build Socket
A separate executable is required for the client and server. Socket uses the Make utility to build each of them.  The following commands should be made from the ./Socket/client and ./Socket/server file paths.
* Enter "make dirs" to create necessary build directories.
* Enter "make" to build the client or server executable.
* Enter "make clean" to remove all build files.

## How to Run Socket
* Ensure superuser privileges are enabled for the user running the program.
* Run the server executable.  The server will open a port and wait for an incoming connection.
* Run the client executable from the command line with any single option of '-m', '-f', '-p' where 'm' is for message, 'f' for file, and 'p' for packet.

## Program Motivation
This was a refresher for socket programming in C.  I wanted to revisit socket programming because I may use some of these features as a base for implementing client/server communication in another program I am working on.

## Warning
Sending hand-coded packets is considered a high-risk operation for system security.  While we are sending empty packets to ourselves using the loopback address, doing so should be done in your own confidence of the code to take such risks.