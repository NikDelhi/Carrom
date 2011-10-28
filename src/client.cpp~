#include "networking.h"

using namespace std;

Client :: Client()
{
	sockfd = -1;
	bindid = -1;
	listenid = -1;
	acceptid = -1;
	connectid = -1;
	maxReceiveLength = 100;
}

void Client :: connectServer(bool isConnectionEstablished)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) cout<<"error opening"<<endl;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(DEST_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
	while(1)
	{
		if(connectid == -1) connectid = connect(sockfd,(struct sockaddr*) &dest_addr,sizeof(struct sockaddr));
		if(connectid == -1) cout<<"error connecting to server"<<endl;
		else
		{
			isConnectionEstablished = true;
			cout<<"connection established to server"<<endl;
			break;
		}
	}
			
}

void Client :: sendMsg(char* msg)
{
	send(sockfd,msg,strlen(msg),0);
}

void Client :: recvMsg(char recvArray[])
{
	recv(sockfd,recvArray,maxReceiveLength,0);
}
