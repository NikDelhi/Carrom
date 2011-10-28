
#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "header.h"


#define DEST_IP "127.0.0.1"
#define DEST_PORT 3120

class Server
{
	private :
		int sockfd;
		int bindid;
		struct sockaddr_in dest_addr;
		struct sockaddr_in client_addr;
		int listenid;
		int acceptid;
		int connectid;
		
	public :
		int clientIds[3];
		int clientCount;
		int maxReceiveLength;
		Server();
		void startServer(bool);
		void sendMsg(char*);
		void recvMsg(char [],int );		
		int recvMsgNonBlock(char [],int );		
		void sendMsg(char* msg,int clientNo);
		void sendMsgLimited(char* msg,int clientNo);
		//void recvMsg(char [],int );
};

class Client
{
	private :
		int sockfd;
		int bindid;
		struct sockaddr_in dest_addr;
		struct sockaddr_in client_addr;
		int listenid;
		int acceptid;
		int connectid;
		int maxReceiveLength;
		
	public :
		Client();
		void connectServer(bool );
		void recvMsg(char []);
		void sendMsg(char* );
		
};

//extern char sendmsg[50];
//extern char recvmsg[50];	
	
#endif
