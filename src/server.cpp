#include "networking.h"
#include "header.h"
/*
int sockfd;
		int bindid;
		struct sockaddr_in dest_addr;
		struct sockaddr_in client_addr;
		int listenid;
		int acceptid;
		int connectid;
*/

using namespace std;

Server :: Server()
{
	sockfd = -1;
	bindid = -1;
	listenid = -1;
	acceptid = -1;
	connectid = -1;
	maxReceiveLength = 100;
	clientCount = 0;
}

void Server :: startServer(bool isConnectionEstablished)
{
	//commid = -1;
	isConnectionEstablished = false;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	cout<<"sockfd is "<<sockfd<<endl;
	if(sockfd == -1) cout<<"error opening"<<endl;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(DEST_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
	bindid = bind(sockfd,(struct sockaddr*) &dest_addr,sizeof(struct sockaddr));
	socklen_t len = sizeof(struct sockaddr);
	//if(listenid == -1) listenid = listen(sockfd,3);
	while(1)
	{
		if(listenid == -1) listenid = listen(sockfd,3);
		if(listenid != -1)
		{
			//cout<<"server established"<<endl;
			//cout<<"listenid is "<<listenid<<endl;
			if(acceptid == -1)
			{
				acceptid = accept(sockfd,(struct sockaddr*) &client_addr,&len);
			}
			if(acceptid != -1)
			{
				isConnectionEstablished =  true;
				//cout<<"no of clients is "<<clientCount<<endl;
				//cout<<"client -1 is "<<clientIds[clientCount-1]<<endl;
				//cout<<"accept id is "<<acceptid<<endl;
				//sleep(1);
				//pid_t pid;
				//pid = fork();
				//if(pid == 0)
				//{
					if((clientCount == 0 || clientIds[clientCount-1] != acceptid) && clientCount < 3) 
					{
						cout<<"incoming client connection"<<endl;
						cout<<"connection established"<<endl;
						//cout<<"sockfd is "<<sockfd<<endl;
						//cout<<"listenid is "<<listenid<<endl;
						//cout<<"acceptid is "<<acceptid<<endl;
						//close(acceptid);
						//acceptid = accept(sockfd,(struct sockaddr*) &client_addr,&len);
						char msg[2];
						msg[0] = 'p';
						//msg[1] = clientCount+49;
						msg[1] = getPlayerId(clientCount+1) + 48;
						//cout<<"player Id is "<<msg[1]<<endl;
						//cout<<"sending "<<msg<<endl;
						send(acceptid,msg,2,0);
						clientIds[clientCount++] = acceptid;
						cout<<"acceptid for clientcount = "<<clientCount<<" is "<<acceptid<<endl; 
						//close(acceptid);
						//acceptid = -1;
						//exit(0);
					}
					acceptid = -1;
					//exit(0);
				//}
				//else
				//{
					//if(clientCount == 1)
					//{
					//	exit(0);
					//}
					//close(acceptid);
					//acceptid = -1;
				//}
				//break;
			}
			else cout<<"error accepting"<<endl;
		}
		else cout<<"error listening"<<endl;
	}
}

void Server :: sendMsg(char* msg)
{
	for(int i = 0; i < clientCount; i++)
	{
		send(clientIds[i],msg,strlen(msg),0);
	}
}

void Server :: sendMsgLimited(char* msg,int no)
{
	for(int i = 0; i < clientCount; i++)
	{
		if(i != no) send(clientIds[i],msg,strlen(msg),0);
	}
}

void Server :: sendMsg(char* msg,int clientNo)
{
	send(clientIds[clientNo],msg,strlen(msg),0);
}

void Server :: recvMsg(char recvArray[],int client)
{
	//for(int client = 0; client < clientCount; client++)
	//{
		cout<<"receiving from id = "<<clientIds[client]<<endl;
		recv(clientIds[client],recvArray,maxReceiveLength,0);
	//}
}

int Server :: recvMsgNonBlock(char recvArray[],int client)
{
	//for(int client = 0; client < clientCount; client++)
	//{
		int success;
		cout<<"receiving from id = "<<clientIds[client]<<endl;
		success = recv(clientIds[client],recvArray,maxReceiveLength,MSG_DONTWAIT);
	//}
}
