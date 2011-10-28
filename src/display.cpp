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

using namespace std;

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
GLint circlePoints = 100;
GLdouble minx;
GLdouble miny;
GLdouble maxx;
GLdouble maxy;
GLdouble zPlane = 0;
GLint maxLayers = 50;
GLdouble layerWidth = 0.01;
GLint maxCircleTriangles = 50;
GLint maxCylinderPoles = 50;
GLint totalDiscs;
GLint winx = 0,winy = 0,winMaxX = 700,winMaxY = 700;
GLdouble minCoord;
GLdouble maxCoord = winMaxX;
Disc *discs;
Wall *walls;
Hole *holes;
Disc striker,striker2;
bool motionOn = true;
int state=1;
double systemTime = 0;
double callingTime = 1.0;
double discsTranslate[10][2];
double strikerTranslate[2] = {0.0,0.0};
double delt = 0.5;
int xi=-1,yi=-1;
double v[2];
double newx,newy;    
Server server;
GLuint texId;
			
void setDiscs(Disc* d)
{
	discs = d;
}
void setStriker(Disc d)
{
	striker.createDisc(d);
	striker2.createDisc(d);
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
void  setMinCoord(double val)
{
	minCoord = val;
}
void  setMaxCoord(double val)
{
	maxCoord = val;
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
Dim*  drawCircle(double cx,double cy,double cz,double r,GLfloat color[])
{
	glColor3fv(color);
	Dim *coords = (Dim*) malloc(maxCircleTriangles*sizeof(Dim));
	int startIndex = 0;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(cx,cy,cz);
	int count;
	double l,x1,y1;
	for(count=0;count<maxCircleTriangles;count++)
	{
		l=(360*1.0/maxCircleTriangles)*count/180 *3.143;
		x1=cx+r*cos(l);
		y1=cy+r*sin(l);
		coords[startIndex].x = x1;
		coords[startIndex++].y = y1;
		glVertex3d(x1,y1,cz);
	}
	glVertex3d(cx+r,cy,cz);
	glEnd();
	return coords;
}
void  drawCircle(double cx,double cy,double cz,double r,GLfloat color[],Dim* c)
{
	glColor3fv(color);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(cx,cy,cz);
	int count;
	for(count=0;count<maxCircleTriangles;count++)
	{
		glVertex3d(c[count].x,c[count].y,cz);
	}
	glVertex3d(cx+r,cy,cz);
	glEnd();
}
void  drawDisc(Disc d,GLfloat color[])
{
	glColor3fv(color);
	Dim *coords;// = malloc(sizeof(Dim)*maxCylinderPoles);
	coords = drawCircle(d.getX(),d.getY(),zPlane,d.getRadius(),color);
	int i;
	for(i = 1; i <= maxCylinderPoles; i++)
	{
		glBegin(GL_LINE);
			glVertex4d(coords[i].x,coords[i].y,zPlane,maxCoord);
			glVertex4d(coords[i].x,coords[i].y,d.getHeight()+zPlane,maxCoord);
		glEnd();	
	}
	drawCircle(d.getX(),d.getY(),zPlane+d.getHeight(),d.getRadius(),color,coords);
}
void  drawRect(double x1,double x2,double y1,double y2,double z1,double z2,GLfloat color[])
{
	glColor3fv(color);
	glBegin(GL_QUADS);
		glVertex3d(x1,y1,z1);
		glVertex3d(x2,y1,z1);
		glVertex3d(x2,y2,z1);
		glVertex3d(x1,y2,z1);
	glEnd();
}
void  drawWall(Wall wall,GLfloat color[])
{
	int i;
	double width = wall.getWidth();
	for(i=0; i < maxLayers; i++)
	{
		if(wall.getPos() == 0)	drawRect(minx-width,maxx+width,wall.getY()-width,wall.getY(),zPlane+i*layerWidth,zPlane+i*layerWidth,color);
		else if(wall.getPos() == 1) drawRect(wall.getX(),wall.getX()+width,miny-width,maxy+width,zPlane+i*layerWidth,zPlane+i*layerWidth,color);
		else if(wall.getPos() == 2) drawRect(minx-width,maxx+width,wall.getY(),wall.getY()+width,zPlane+i*layerWidth,zPlane+i*layerWidth,color);
		else drawRect(wall.getX()-width,wall.getX(),miny-width,maxy,zPlane+i*layerWidth,zPlane+i*layerWidth,color);
	}
}
void  drawHole(Hole h,GLfloat color[])
{
	//cout<<h.getX()<<" "<<h.getY()<<" "<<h.getRadius()<<endl;
	drawCircle(h.getX(),h.getY(),zPlane,h.getRadius(),color);
}
//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
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
	return textureId; //Returns the id of the texture
}

void  init(void) 
{
    //glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Image* image = loadBMP("Carrom.bmp");
	texId = loadTexture(image);
	delete image;
    //glDisable(GL_ANTIALIASING);
    //glCallList(carrom);
    v[0]=0;
    v[1]=0;
    motionOn=0;
    drawCarrom();
    striker.setRadius(striker_radius);	
    striker2.setRadius(striker_radius);	
    for(int i=0;i<totalDiscs;i++)		discs[i].setRadius(disc_radius);
    striker.updateCoords(0,(strikerHigh+strikerLow)/2);
    striker2.updateCoords(0,(strikerHigh2+strikerLow2)/2);
    newx=striker.getX();
    newy=striker.getY(); 
}
bool decodeInfo(char recvmsg[])
{
	//char *recvmsg = (char*) malloc(50);
	int receivingBytes = 0;	
	//receivingBytes = recv(acceptid,recvmsg,50,0);
	//cout<<recvmsg<<endl;
	//motionOn = false;
	if(strcmp(recvmsg,"stop"))
	{	
		return true;	
	}
	else
	{
		char *p = (char*) malloc(strlen(recvmsg)-1);
		p = &recvmsg[1];	
		if(recvmsg[0] == 'b')
		{
			//double coord = atof(p);
			char *sep = strchr(p,'a');
			int len = strlen(p)-strlen(sep);
			char *barxstr = (char*) malloc(len);
			double bary = atof(sep+1);
			strncpy(barxstr,p,len);
			double barx = atof(barxstr);
			drawArrow(barx,bary,1);
		}
		else
		{
			double xc = atof(p);
			striker2.updateCoords(xc,striker2.getY());
		}		
		return false;			
	}

}
char* encodeInfo(char c,int x,int y)
{
	char* msg = (char*) malloc(server.maxReceiveLength);
	if(c == 's')
	{
		sprintf(msg,"s%lf",newx);
	}
	else if(c == 'm')
	{
		sprintf(msg,"%da%d",x,y);
	}
	else if(c == 'b')
	{
		sprintf(msg,"b%lfa%lf",newx,newy);
	}
	else
	{
		sprintf(msg,"stop");
	}
	return msg;
}
void  display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glCallList(carrom);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	glBegin(GL_QUADS);	
	//glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(minx, miny, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(maxx, miny, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(maxx, maxy, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(minx, maxy, 0);	
	glEnd();  
	
   if(state==1)
   drawArrow(newx,newy,0);
   int i;
   for(i=0; i < 4; i++)
   {
		drawWall(walls[i],darkbrown);
   }
   for(i=0; i < totalDiscs; i++)
   {
		drawDisc(discs[i],black);
		glLoadIdentity();			
   }
   glTranslated((GLdouble) strikerTranslate[0],(GLdouble) strikerTranslate[1],0);
   drawDisc(striker,red);
   drawDisc(striker2,yellow);
   //cout<<striker.getX()<<" "<<striker.getY()<<endl;
   //cout<<striker2.getX()<<" "<<striker2.getY()<<endl;
   glLoadIdentity();
   glutSwapBuffers();
}
void animate()
{
	if(motionOn)
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
				//cout<<"striker falling in hole"<<endl;
				//motionOn = 0;
				//sleep(3);
				//motionOn = 1;
			}
			for(int j = 0; j < totalDiscs; j++)
			{
				if(isDiscInside(discs[j],holes[i]))
				{
					//totalDiscs-=1;
					for(int i=j;i<totalDiscs-1;i++)			discs[i]=discs[i+1];
					totalDiscs-=1;
					//cout<<"disc "<<discs[j].getDiscNo()<<" falling into hole"<<endl;
					//motionOn = 0;
					//sleep(3);
					//motionOn = 1;
				}
			}
		}
		
		striker.updateDisc(delt);
		for(int i=0; i < totalDiscs; i++)
		{
			discs[i].updateDisc(delt);
		}
		glutPostRedisplay();
	}
}
void  reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //gluPerspective(60,(GLfloat)w/(GLfloat)h,0.1f,10.0f);
   glOrtho(-maxCoord/2, maxCoord/2, -maxCoord/2, maxCoord/2, -maxCoord/2, maxCoord/2);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
void drawArrow(double newx,double newy,int no)
{
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	if(no == 0) glVertex2f(striker.getX(),striker.getY());
	else glVertex2f(striker2.getX(),striker2.getY());
	glVertex2f(newx,newy);
	glEnd();
	
}	
void mouse(int b,int s,int x,int y)
{
		
	switch(b)
	{
	
		case GLUT_RIGHT_BUTTON:
			if(s==GLUT_DOWN){
			motionOn=1-motionOn;}
			break;
		case GLUT_LEFT_BUTTON:
			motionOn=1;
			switch(s)
			{
				case GLUT_DOWN:	
					//glutIdleFunc(NULL);
					state=1;
					motionOn=1;
					cout<<"processing"<<state<<"\n";
					glutPostRedisplay();
					break;
				case GLUT_UP:
					glutIdleFunc(animate);
					server.sendMsg(encodeInfo('f',0,0));
					cout<<"sending message "<<encodeInfo('f',0,0)<<endl;
					cout<<"done\n"<<state<<"\n";
					if(state==0) break;
					striker.updateDisc(v);
					cout<<v[0]<<" "<<v[1]<<"\n";
					v[0]=0;
					v[1]=0;
					state=0;
					motionOn=1;
					break;
			}
	}
	
	
}
void mouseMotion(int x,int y)
{
	server.sendMsg(encodeInfo('m',x,y));
	cout<<"sending message"<<encodeInfo('m',x,y)<<endl;
	cout<<"to be sent "<<x<<" "<<y<<endl;
	//strikerTranslate[0] = x-xi;
	double l,slope;
	newx = -(winMaxX/2 - x);
	newy = (winMaxY/2 - y);
	//cout<<"new y"<<newy<<" "<<y<<"y\n";
	if(newy >= striker.getY()-striker.getRadius() && newy<= striker.getY()+striker.getRadius()&&newx >= striker.getX()-striker.getRadius() && newx<= striker.getX()+striker.getRadius()&& newx < base_line_length/2 && newx>-base_line_length)
	{
		striker.updateCoords(newx,striker.getY());
		//server.sendMsg(encodeInfo('s',newx,0));
		//cout<<"sending message"<<encodeInfo('s',newx,0)<<endl;
	}
	//////CONDITION FOR STRIKER PLACEMENT NEED TO BE CORRECTED ACCORDING TO RULES/////
	double ymax = striker.getY();// - striker.getRadius();
	if(newy>striker.getY()-striker.getRadius())
	return;
	double y1 = striker.getY() - newy;
	double x1 = striker.getX() - newx;
	drawArrow(newx,newy,0);
	//server.sendMsg(encodeInfo('b',newx,newy));
	//cout<<"sending message"<<encodeInfo('b',newx,newy)<<endl;
	l=(x1*x1+y1*y1)*.003 ;
	l=(l>12)?12:l;
	//slope=y1/x1;
	double a = 4;
	v[0]=x1*l/sqrt(x1*x1+y1*y1);
	v[1]=y1*l/sqrt(x1*x1+y1*y1);
	glutPostRedisplay();
}		
void  setLinks(int& argc,char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB );
   glutInitWindowSize (winMaxX, winMaxY); 
   glutInitWindowPosition (winx, winy);
   glutCreateWindow ("WELCOME TO WORLD OF CARROM");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
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
	//server = new Server();
	bool isConnected;
	server.startServer(isConnected);
	glutMainLoop();
}



