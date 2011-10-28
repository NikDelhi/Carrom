#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <queue>
#include <string.h>
#include "networking.h"
#include "dispList.h"
#include "physics.h"
#include "header.h"
#include "imageloader.h"
#include <pthread.h>

using namespace std;

#define xnew(a,x,y) (x*cos(a) - y*sin(a))
#define ynew(a,x,y) (y*cos(a) + x*sin(a))
#define moveunit .02
#define moveangleunit (M_PI/180) 

GLfloat black[] = {0.0,0.0,0.0};
GLfloat darkbrown[] = {0.6,0.3,0.2};
GLfloat brown[] = {0.9,0.5,0.3};
GLfloat red[] = {1.0,0.0,0.0};
GLfloat orange[] = {1.0,0.5,0.0};
GLfloat yellow[] = {1.0,1.0,0.0};
GLfloat green[] = {0.0,1.0,0.0};
GLfloat blue[] = {0.0,0.0,1.0};
GLfloat gray[] = {0.5,0.5,0.5};
GLfloat cyan[] = {0.0,1.0,1.0};
GLfloat purple[] = {0.5,0.0,1.0};
GLfloat magenta[] = {1.0,0.0,0.5};
GLfloat pink[] = {1.0,0.0,1.0};	
GLfloat white[] = {1.0,1.0,1.0};
GLdouble minx;
GLdouble miny;
GLdouble maxx;
GLdouble maxy;
GLdouble winMaxX = 700,winMaxY = 700;
GLdouble zPlane = 0;
GLint totalDiscs;
GLint maxCircleTriangles = 50;
Disc *discs;
Wall *walls;
Hole *holes;
Disc striker;
bool motionOn = true;
double delt = 0.009;
int texNo = 1;
GLuint textureId[5],boardId,woodId,whiteId,blackId,strikerId;
GLdouble lookx = 0,looky = -1,lookz = 1,lookr = .2,angle=5*M_PI/180;
int currentPlayer=0;
int totalplayers=1;
int playerIds[4];
bool strikerLocked=false;
bool arrowLocked = true;
Player players[4];
double arrow[2];
double power=0;
double unitPower=.03;
double maxPower=1;
int enterCount = 0;
int prevState = 0;
int playerId = 0;	
bool isServer = false;
bool isClient = false;	
Server server;
Client client;
int defaultPlayer = 0;

void setDiscs(Disc* d)
{
	discs = d;
}
void setStriker(Disc d)
{
	striker.createDisc(d);
}
void setWalls(Wall* w)
{
	walls = w;
}
void setHoles(Hole* h)
{
	holes = h;
}
void  setTotalDiscs(int n)
{
	totalDiscs = n;
}
void  setminx(double val)
{
	minx = val;
}
void  setmaxx(double val)
{
	maxx = val;
}
void  setminy(double val)
{
	miny = val;
}
void  setmaxy(double val)
{
	maxy = val;
}
GLuint loadTexture(Image* image,GLuint texture[],int i) {
//	GLuint textureId;
//	glGenTextures(1, &textureId); //Make room for our texture
//	printf("TextureIdGenerated = %d\n", textureId);
	glBindTexture(GL_TEXTURE_2D, texture[i]); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return texture[i]; //Returns the id of the texture
}
void init()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
    //glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(5,textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    Image* image = loadBMP("wood.bmp");
	woodId = loadTexture(image,textureId,0);
	delete image;
    image = loadBMP("carrom2.bmp");
	boardId = loadTexture(image,textureId,1);
	delete image;
    image = loadBMP("striker.bmp");
	strikerId = loadTexture(image,textureId,2);
	delete image;
    image = loadBMP("white.bmp");
	whiteId = loadTexture(image,textureId,3);
	delete image;
    image = loadBMP("black.bmp");
	blackId = loadTexture(image,textureId,4);
	delete image;    
	playerIds[0] = 0;
	playerIds[1] = 1;
	playerIds[2] = 2;
	playerIds[3] = 3;
	for(int i=0; i < totalplayers; i++)
	{
		players[playerIds[i]].setPlayer(playerIds[i]);
	}
	striker.updateCoords(players[currentPlayer].strikerx,players[currentPlayer].strikery);
	arrow[0] = players[currentPlayer].arrowx;
	arrow[1] = players[currentPlayer].arrowy;
	lookx = players[playerIds[0]].lookx;
	looky = players[playerIds[0]].looky;
	
}

void drawCube()
{
   glBegin(GL_QUADS); 


   // Bottom Face.  Red, 75% opaque, magnified texture
 
   glNormal3f( 0.0f, -1.0f, 0.0f); // Needed for lighting
   //glColor4f(0.9,0.2,0.2,.75); // Basic polygon color

   glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f); 
   glTexCoord2f(1, 0); glVertex3f( 1.0f, -1.0f, -1.0f);
   glTexCoord2f(1, 1); glVertex3f( 1.0f, -1.0f,  1.0f);
   glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f,  1.0f);


   // Top face; offset.  White, 50% opaque.
 
   glNormal3f( 0.0f, 1.0f, 0.0f);  //glColor4f(0.5,0.5,0.5,.5);

   glTexCoord2f(0, 0); glVertex3f(-1.0f,  1.0f, -1.0f);
   glTexCoord2f(1, 0); glVertex3f(-1.0f,  1.0f,  1.0f);
   glTexCoord2f(1, 1); glVertex3f( 1.0f,  1.0f,  1.0f);
   glTexCoord2f(0, 1); glVertex3f( 1.0f,  1.0f, -1.0f);


   // Far face.  Green, 50% opaque, non-uniform texture cooridinates.

   glNormal3f( 0.0f, 0.0f,-1.0f);  //glColor4f(0.2,0.9,0.2,.5); 

   glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
   glTexCoord2f(1, 0); glVertex3f(-1.0f,  1.0f, -1.0f);
   glTexCoord2f(1, 1); glVertex3f( 1.0f,  1.0f, -1.0f);
   glTexCoord2f(0, 1); glVertex3f( 1.0f, -1.0f, -1.0f);


   // Right face.  Blue; 25% opaque
   
   glNormal3f( 1.0f, 0.0f, 0.0f);  //glColor4f(0.2,0.2,0.9,.25);

   glTexCoord2f(0, 0); glVertex3f( 1.0f, -1.0f, -1.0f); 
   glTexCoord2f(1, 0); glVertex3f( 1.0f,  1.0f, -1.0f);
   glTexCoord2f(1, 1); glVertex3f( 1.0f,  1.0f,  1.0f);
   glTexCoord2f(0, 1); glVertex3f( 1.0f, -1.0f,  1.0f);


   // Front face; offset.  Multi-colored, 50% opaque.

   glNormal3f( 0.0f, 0.0f, 1.0f); 

   //glColor4f( 0.9f, 0.2f, 0.2f, 0.5f);
   glTexCoord2f(0.25f, 0.25f); glVertex3f(-1.0f, -1.0f,  1.0f);
   //glColor4f( 0.2f, 0.9f, 0.2f, 0.5f);
   glTexCoord2f(1.f, 0.25f); glVertex3f( 1.0f, -1.0f,  1.0f);
   //glColor4f( 0.2f, 0.2f, 0.9f, 0.5f);
   glTexCoord2f(1.f, 1.f); glVertex3f( 1.0f,  1.0f,  1.0f); 
   //glColor4f( 0.1f, 0.1f, 0.1f, 0.5f);
   glTexCoord2f(0.25f, 1.f); glVertex3f(-1.0f,  1.0f,  1.0f);


   // Left Face; offset.  Yellow, varying levels of opaque.
   
   glNormal3f(-1.0f, 0.0f, 0.0f);  
   
   //glColor4f(0.9,0.9,0.2,0.0);
   glTexCoord2f(0.f, 0.f); glVertex3f(-1.0f, -1.0f, -1.0f); 
   //glColor4f(0.9,0.9,0.2,0.66);
   glTexCoord2f(1.f, 0.f); glVertex3f(-1.0f, -1.0f,  1.0f);
   //glColor4f(0.9,0.9,0.2,1.0);
   glTexCoord2f(1.f, 1.f); glVertex3f(-1.0f,  1.0f,  1.0f);
   //glColor4f(0.9,0.9,0.2,0.33);
   glTexCoord2f(0.f, 1.f); glVertex3f(-1.0f,  1.0f, -1.0f);


   // All polygons have been drawn.
   glEnd();

}
void drawWalls()
{
	GLdouble w=walls[0].getWidth(),l=walls[0].getLength()+2*w,h=walls[0].getHeight();
	GLdouble b = (maxx - minx);
	//float l=0.8,w=0.15,h=.2,b=.5;
	
	
	
	//glColor3f(1.0,0.0,0.0);
	glPushMatrix();
	glTranslatef(0,b/2+w/2,h/2);
	glScalef(l/2,w/2,h/2);
	drawCube();
	glPopMatrix();
	
	//glColor3f(0.0,0.0,1.0);
	glPushMatrix();
	glTranslatef(b/2+w/2,0,h/2);
	glScalef(w/2,l/2,h/2);
	drawCube();
	glPopMatrix();
	
	//glColor3f(0.0,1.0,0.0);
	glPushMatrix();
	glTranslatef(0,-(b/2+w/2),h/2);
	glScalef(l/2,w/2,h/2);
	drawCube();
	glPopMatrix();
	
	//glColor3f(0.0,0.0,0.0);
	glPushMatrix();
	glTranslatef(-(b/2+w/2),0,h/2);
	glScalef(w/2,l/2,h/2);
	drawCube();
	glPopMatrix();
	
}
void drawBoard()
{
	glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0f); 

    glTexCoord2f(0.02f, 0.02f); glVertex3f(minx, miny,  0);
    glTexCoord2f(.98f, .02f); glVertex3f( maxx, miny,  0);
    glTexCoord2f(.98f, .98f); glVertex3f( maxx,  maxy,  0); 
    glTexCoord2f(.02f, .98f); glVertex3f(minx,  maxy,  0);
    glEnd();

}
void errorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf(stderr, "Quadric Error: %s\n", estring);
   exit(0);
}
void  drawCircle(Disc d)
{
	double cx = d.getX(),cy = d.getY(),cz = d.getHeight(),r = d.getRadius();
	//glColor3fv(color);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);glVertex3d(cx,cy,cz);
	int count;
	double l,x1,y1;
	for(count=0;count<maxCircleTriangles;count++)
	{
		l=(360*1.0/maxCircleTriangles)*(count*1.0/180)*3.143;
		x1=cx+r*cos(l);
		y1=cy+r*sin(l);
		glTexCoord2f(.5+.5*cos(l),.5+.5*sin(l));glVertex3d(x1,y1,cz);
	}
	glTexCoord2f(1,.5);glVertex3d(cx+r,cy,cz);
	glEnd();
}
void drawCoin(Disc d,GLfloat color[],int tex)
{
	//cout<<" tex "<<tex<<" ";
	GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL); /* flat shaded */
    gluQuadricNormals(qobj, GLU_SMOOTH);
    glColor3fv(color);
    glPushMatrix();
    glTranslatef(d.getX(),d.getY(),0);
    gluCylinder(qobj, d.getRadius(), d.getRadius(), d.getHeight(), 20, 25);
    glPopMatrix();
    //glColor3fv(white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(tex != 2) glBindTexture(GL_TEXTURE_2D, blackId);//i%2==1?whiteId:);//blackId);
	else glBindTexture(GL_TEXTURE_2D, strikerId);//i%2==1?whiteId:);//blackId);
	drawCircle(d);
	
}
void drawArrow()
{
	glBegin(GL_LINES);
	glVertex3f(striker.getX(),striker.getY(),striker.getHeight());
	glVertex3f(arrow[0],arrow[1],striker.getHeight());
	glEnd();
}
bool checkEndTurn()
{
	if(striker.isAtRest())
	{
		for(int i = 0; i < totalDiscs; i++)
		{
			if(!discs[i].isAtRest()) return false;
		}
		return true;
	}
	return false;
}
void powerbar()
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(0.0,0.0,0.0);
	int tsteps = maxPower/(5*unitPower);
	double start = -.5;
	double end = .5;
	double unitl = (end-start)/tsteps;
	for(double i=0;i<power;i+=5*unitPower)
	{
		glColor3f(1.0,1.0,0.0);
		glVertex3f(start+i*unitl,0,1);
		glVertex3f(start+(i+1)*unitl,0,1);
		glVertex3f(start+(i+1)*unitl,0,.6);
		glVertex3f(start+i*unitl,0,.6);
		
	}	
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(lookx,looky,lookz,0,0,0,0,0,1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, woodId);
	drawWalls();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, boardId);
	drawBoard();
	for(int i=0; i < totalDiscs; i++)
	{
		//cout<<"i "<<i<<" ";
		drawCoin(discs[i],i%2==0?black:brown,i%2==0?0:1);
	
	}
	//double stkCoords[2];
	drawCoin(striker,white,2);
	if(!arrowLocked) drawArrow();
	powerbar();
	glutSwapBuffers();
}
void* startServer(void* p)
{
	bool connec;
	server.startServer(connec);
}
void sendmsg(char* p)
{
	if(isServer)
	{
		server.sendMsg(p);
	}
	else if(isClient)
	{
		client.sendMsg(p);
	}
}
void* recvmsg(void* p)
{
	//cout<<"receiving "<<endl;
	char msg[100];
	int senderId;
	while(isServer || isClient)
	{
		if(isServer)
		{
			if(totalplayers > 1)
			{
				if(playerId > 0)
				{
					cout<<"trying to receive from client"<<endl;	
					int success;
					success = server.recvMsgNonBlock(msg,getTotalPlayers(playerId)-2);
					if(success == -1) continue;
					cout<<"server receiving message "<<msg<<endl;
					server.sendMsgLimited(msg,getTotalPlayers(playerId)-2);
				}
			}
			else continue;
		}
		else if(isClient)
		{
			client.recvMsg(msg);
			//cout<<msg<<endl;
		}
		else continue;
		if(strlen(msg) > 2)
		{
			double arr[2];
			char* p = (char*) malloc(strlen(msg)-1);
			p = &msg[1];
			char* sep = strchr(msg,'x');
			int len = strlen(p)-strlen(sep);
			char* q = (char*) malloc(len);
			strncpy(q,p,len);
			arr[0] = atof(q);	
			arr[1] = atof(sep+1);
			if(msg[0] == 's')
			{
				striker.updateCoords(arr);
			}
			else if(msg[0] == 'a')
			{
				arrow[0] = arr[0];
				arrow[1] = arr[1];
			}
			else if(msg[0] == 'v')
			{
				strikerLocked = true;
				striker.updateDisc(arr);
			}
			else if(msg[0] == 'z')
			{
				strikerLocked = false;
			}
		}
		else if(msg[0] == 'p')
		{
			//cout<<"received "<<msg<<endl;
			int id = atoi(&msg[1]);
			/*playerIds[0] = id;
			players[id].setPlayer(id);
			playerIds[1] = 0;
			players[0].setPlayer(0);
			//totalplayers = getPlayerIndex(id)+1;
			totalplayers = 2;
			playerId = 1;*/
			defaultPlayer = id;
			totalplayers = getTotalPlayers(id);
			playerId = 0;
			for(int i = 0; i < 4; i++)
			{
				players[i].setPlayer(i);
			}
			lookx = players[id].lookx;
			looky = players[id].looky;
			/*if(id == 1)
			{
				playerIds[2] = 2;
				players[2].setPlayer(2);
				totalplayers++;
			}
			else if(id == 3)
			{
				playerIds[2] = 1;
				players[2].setPlayer(1);
				playerIds[3] = 2;
				players[3].setPlayer(2);
				totalplayers = 4;
			}*/
	
		}
		else if(msg[0] == 'n')
		{
			//playerIds[totalplayers] = getPlayerId(totalplayers);
			totalplayers++;
		}
	}
}
int getPlayerId(int index)
{
	switch(index)
				{
					case 0:
						return 0;
					case 1:
						return 2;
					case 2:
						return 1;
					case 3:
						return 3;
				}
}
int getTotalPlayers(int pos)
{
	switch(pos)
	{
		case 0:
			return 1;
		case 1:
			return 3;
		case 2:
			return 2;
		case 3:
			return 4;
	}
}
void* playerCheck(void* p)
{
	//char msg[1];
	//cout<<"in player check"<<endl;
	while(1)
	{
		//cout<<"in while"<<endl;
		if(isServer)
		{
			//cout<<"in 1st if"<<endl;
			if(server.clientCount > totalplayers-1)
			{
				//cout<<"in 2nd if"<<endl;
				char msg[1];
				msg[0] = 'n';
				for(int i = 0; i < totalplayers-1; i++)
				{
					server.sendMsg(msg,i);
				}
				for(int i=totalplayers;i<=server.clientCount;i++)
				{
					//cout<<"in for"<<endl;
					//playerIds[i] = getPlayerId(i);
					//msg[0] = playerIds[i];
					
				}
				totalplayers = server.clientCount+1;
			}
		}
	}
}
void encodeInfo(char ch,double arr[],char msg[])
{
	sprintf(msg,"%c%lfx%lf",ch,arr[0],arr[1]);
}
void keyboard(unsigned char key,int x,int y)
{
	//cout<<"key pressed"<<endl;
	double xtemp = lookx,ytemp = looky;
	switch(key)
	{
		case 'w':
			//cout<<"w"<<endl;
			looky -= lookr*ytemp/(ytemp*ytemp+xtemp*xtemp);
			lookx -= lookr*xtemp/(ytemp*ytemp+xtemp*xtemp);
			break;
		case 's':
			looky += lookr*ytemp/(ytemp*ytemp+xtemp*xtemp);
			lookx += lookr*xtemp/(ytemp*ytemp+xtemp*xtemp);
			break;
		case 'a':
			lookx = xnew(-angle,lookx,looky);
			looky = ynew(-angle,lookx,looky);
			break;
		case 'd':
			lookx = xnew(angle,lookx,looky);
			looky = ynew(angle,lookx,looky);
			break;
		case 't':
			lookz += .2;
			break;
		case 'g':
			lookz -= .2;
			break;
		case 13:
			enterCount++;
			if(enterCount == 1 && checkEndTurn()) 
			{
				strikerLocked = true;
				arrowLocked = false;
			}
			else 
			{
				strikerLocked = false;
				arrowLocked = true;
				power = 0;
				enterCount = 0;
				char msg[100];
				double arr[2] = {0,0};
				encodeInfo('z',arr,msg);
				sendmsg(msg);
			}
			//cout<<"striker lock is "<<strikerLocked<<endl;
			break;
	}
	glutPostRedisplay();
	
}
void spKeyboard(int key,int x,int y)
{
	double stkCoords[2];
	double velVector[2];
	char msg[100];
	if(playerIds[playerId] == defaultPlayer)
	{
		switch(key)
		{
			case GLUT_KEY_LEFT :
				if(!strikerLocked) 
				{
					players[currentPlayer].moveLeft(moveunit,stkCoords);
					striker.updateCoords(stkCoords[0],stkCoords[1]);
					encodeInfo('s',stkCoords,msg);
					sendmsg(msg);
				}
				else 
				{
					players[currentPlayer].moveArrowLeft(moveangleunit,arrow);
					encodeInfo('a',arrow,msg);
					sendmsg(msg);
				}
				break;
			case GLUT_KEY_RIGHT :
				if(!strikerLocked) 
				{
					players[currentPlayer].moveRight(moveunit,stkCoords);
					striker.updateCoords(stkCoords[0],stkCoords[1]);
					encodeInfo('s',stkCoords,msg);
					sendmsg(msg);
				}
				else 
				{
					players[currentPlayer].moveArrowRight(moveangleunit,arrow);
					encodeInfo('a',arrow,msg);
					sendmsg(msg);
				}
				break;
			case GLUT_KEY_UP :
				if(strikerLocked && power<=maxPower)
				{
					power += unitPower;
					players[currentPlayer].setVelocity(velVector,power);
					striker.updateDisc(velVector);
					encodeInfo('v',velVector,msg);
					sendmsg(msg);
				}
				break;
			case GLUT_KEY_DOWN :
				if(strikerLocked && power > 0)
				{
					power -= unitPower;
					players[currentPlayer].setVelocity(velVector,power);
					striker.updateDisc(velVector);
					encodeInfo('v',velVector,msg);
					sendmsg(msg);
				}
				break;
			case GLUT_KEY_HOME :
				isServer = true;
				isClient = false;
				cout<<"processing the request to become server"<<endl;
				pthread_t serverThread,receiveThread,playerCheckThread;
				void* p;
				pthread_create(&serverThread,NULL,startServer,p);
				pthread_create(&receiveThread,NULL,recvmsg,p);
				pthread_create(&playerCheckThread,NULL,playerCheck,p);
				break;
			case GLUT_KEY_END :
				isClient = true;
				isServer = false;
				bool conn;
				client.connectServer(conn);
				pthread_t clientThread;
				pthread_create(&clientThread,NULL,recvmsg,p);
				break;
		}	
	}
	glutPostRedisplay();
}
void animate()
{
	//cout<<"current player is "<<currentPlayer<<" "<<"striker locked?"<<strikerLocked<<endl;
	//cout<<"velocity of striker "<<striker.getVx()<<" "<<striker.getVy()<<endl;
	if(!strikerLocked)
	{
		int collidingDiscs[(totalDiscs)*(totalDiscs+1)][2];// = (Disc*) malloc((totalDiscs)*(totalDiscs+1) * sizeof(Disc));
		int totalColliding = checkDiscCollisions(discs,walls,totalDiscs,striker,collidingDiscs);
		for(int i = 0; i < totalColliding; i++)
		{
			if(collidingDiscs[i][1] >= -1)
			{
				Event *ev = new Event(discs[collidingDiscs[i][0]],collidingDiscs[i][1]==-1?striker:discs[collidingDiscs[i][1]]);
				ev->updateDiscs();	
			}
			else
			{
				wallEvent *wev = new wallEvent(collidingDiscs[i][0]==-1?striker:discs[collidingDiscs[i][0]],walls[collidingDiscs[i][1]+5]);
				wev->updateDiscs();
			}
		}
		for(int i=0; i < 4; i++)
		{
			if(isDiscInside(striker,holes[i]))
			{
				double v[2];
				v[1]=v[0]=0;
				striker.updateDisc(v);
			}
			for(int j = 0; j < totalDiscs; j++)
			{
				if(isDiscInside(discs[j],holes[i]))
				{
					for(int i=j;i<totalDiscs-1;i++)	discs[i]=discs[i+1];
					totalDiscs-=1;
				}
			}
		}
		striker.updateDisc(delt);
		for(int i=0; i < totalDiscs; i++)
		{
			discs[i].updateDisc(delt);
		}
		if(checkEndTurn() && prevState)
		{
			
			playerId = (playerId==totalplayers-1?0:playerId+1);
			sleep(1);
			currentPlayer = playerIds[playerId];
			players[currentPlayer].setPlayer(currentPlayer);
			striker.updateCoords(players[currentPlayer].strikerx,players[currentPlayer].strikery);
			arrow[0] = players[currentPlayer].arrowx;
			arrow[1] = players[currentPlayer].arrowy;
			prevState = 0;
			cout<<"playerId is "<<playerId<<endl;
			cout<<"player is "<<playerIds[playerId]<<endl;
			cout<<"totalplayers are "<<totalplayers<<endl;
		}
		else if(!checkEndTurn())
		{
			prevState = 1;
		}
									
	}
	glutPostRedisplay();
}
void mouse(int b,int s,int x,int y)
{

}
void mouseMotion(int x,int y)
{

}

void  reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45,(GLfloat)w/(GLfloat)h,0.1f,10.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
void  setLinks(int& argc,char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (winMaxX, winMaxY); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("WELCOME TO WORLD OF CARROM");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(spKeyboard);
   glutIdleFunc(animate);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   //glutTimerFunc(callingTime,newEvent,0);
}
void  setIdleFunc(void func(void))
{
	glutIdleFunc(func);
}
void  setMouseFunc(void mouse(int b,int s,int w,int h))
{
	glutMouseFunc(mouse);
}
void  setMouseMotionFunc(void mouseMotion(int w,int h))
{
	glutMotionFunc(mouseMotion);
}
void  setKeyFunc(void Keyboard(unsigned char k,int x,int y))
{
	glutKeyboardFunc(Keyboard);
}
void  startGame()
{
	glutMainLoop();
}
