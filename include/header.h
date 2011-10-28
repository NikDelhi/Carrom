
#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include <GL/glut.h>

using namespace std;

class Disc
{
	private :
		int discNo;
		double m;
		double radius;
		double height;
		double x;
		double y;
		double z;
		double Vx;
		double Vy;
		double Vz;
		double Ax;
		double Ay;
		double Az;
		double latestUpdateTime;
	public :
		int getDiscNo();
		double getMass();
		double getRadius();
		double getHeight();
		double getX();
		double getY();
		double getZ();
		double getVx();
		double getVy();
		double getVz();
		double getAx();
		double getAy();
		double getAz();
		void setAx(double );
		void setAy(double );
		void updateDisc(double v[]);
		void updateDisc(double ,double );
		void updateDisc(double X,double Y,double velx,double vely,double accx,double accy,double time);
		void createDisc(double ,int n,double r,double h,double X,double Y,double velx,double vely,double accx,double accy,double time);
		void createDisc(Disc d);
		void setTime(double time);
		double getTime();
		void updateDisc(double t);
		void updateDisc();
		void updateCoords(double ,double );
		void updateCoords(double []);
		void setRadius(float);
		bool isAtRest();
		//bool checkFall(Hole holes[]);
};

class Hole
{
	private :
		double x;
		double y;
		double radius;
	public :
		double getX();
		
		double getY();
		
		double getRadius();
		
		void createHole(double x,double y,double r);
		
		//bool isDiscIn(Disc d);		
};

//bool Disc :: checkFall(Hole holes[]);

class Wall
{
	private :
		int no;
		Hole hole1;
		Hole hole2;
		double length;
		double width;
		double height;
		double x;
		double y;
		double z;
	public :
		int getPos();
		
		Hole getHole1();
		
		Hole getHole2();
		
		double getLength();
		
		double getWidth();
		
		double getHeight();
		
		double getX();
		
		double getY();
		
		double getZ();
		
		void setZ(double c);
		
		void createWall(int ,Hole ,Hole ,double ,double ,double ,double ,double ,double ,double);
		
};

class Event
{
	private :
		Disc& disc1;
		Disc& disc2;
	protected :
		double timeLeft;
	public :
		//Event();
		Event(Disc& ,Disc& );
		Disc getDisc1();
		Disc getDisc2();
		double getTimeLeft();
		void setEvent(Disc& d1,Disc& d2,double t);
		void setDiscs(Disc& d1,Disc& d2);
		void setTime(double t);
		void calculateEvent(Disc& d1,Disc& d2);
		void updateDiscs();
};

class wallEvent// : public Event
{
	public :
		Disc& disc;
		Wall wall;
		double timeLeft;
		wallEvent(Disc& ,Wall);
		Disc getDisc();
		Wall getWall();
		void calculateEvent(Disc& d,Wall& w);
		void calculateEvent(Disc& d,Wall w[]);
		double calculateEvent(Disc& d,Wall w[],int no);
		void updateDiscs();
		void setEvent(Disc& d, Wall& w);
};

class Dim
{
	public :
		double x;
		double y;
};

/*class Display
{
	private :*/
		extern GLfloat black[3];
		extern GLfloat darkbrown[3];
		extern GLfloat brown[3];
		extern GLfloat red[3];
		extern GLfloat orange[3];
		extern GLfloat yellow[3];
		extern GLfloat green[3];
		extern GLfloat blue[3];
		extern GLfloat gray[3];
		extern GLfloat cyan[3];
		extern GLfloat purple[3];
		extern GLfloat magenta[3];
		extern GLfloat pink[3];	
		extern GLfloat white[3];	
		/*GLint circlePoints;
		GLdouble minCoord;
		GLdouble maxCoord;
		GLdouble zPlane;
		GLint maxLayers;
		GLdouble layerWidth;
		GLint maxCircleTriangles;
		GLint maxCylinderPoles;
		GLint totalDiscs;
		GLint winx,winy,winMaxX,winMaxY;
		Disc *discs;
		Wall *walls;
		Hole *holes;
	public :*/
		//Display();
		void setDiscs(Disc *d);
		void setWalls(Wall *w);
		void setHoles(Hole *h);
		void setTotalDiscs(int n);
		void setMinCoord(double val);
		void setStriker(Disc d);
		void setMaxCoord(double val);
		void  setminx(double val);
		void  setmaxx(double val);
		void  setminy(double val);
		void  setmaxy(double val);
		Dim* drawCircle(double cx,double cy,double cz,double r,GLfloat color[]);
		void drawCircle(double cx,double cy,double cz,double r,GLfloat color[],Dim* c);
		void drawDisc(Disc d,GLfloat color[]);
		void drawRect(double x1,double x2,double y1,double y2,double z1,double z2,GLfloat color[]);
		void drawWall(Wall wall,GLfloat color[]);
		void drawHole(Hole h,GLfloat color[]);
		void init(void) ;
		void display(void);
		void reshape(int w, int h);
		void setLinks(int& argc,char **argv);
		void setIdleFunc(void func(void));
		void setMouseFunc(void mouse(int b,int s,int w,int h));
		void setKeyFunc(void Keyboard(unsigned char k,int x,int y));
		void startGame();
		void drawArrow(double,double,int );
		int getPlayerId(int );
		int getTotalPlayers(int );
		
class Player
{
	public:
		int score;
		double strikerx;
		double strikery;
		double arrowx;
		double arrowy;
		double lookx;
		double looky;
		int playerno;
		void moveLeft(double amt,double newCoords[]);
		void moveArrowLeft(double angle,double newCoords[]);
		void moveRight(double amt,double newCoords[]);
		void moveArrowRight(double angle,double newCoords[]);
		void setPlayer(int no);
		void setVelocity(double v[],double );
};

//};
/*
class CompareEvents
{
	public :
		bool operator()(Event &e1,Event &e2);
		

};*/
//priority_queue<Event,vector<Event>,CompareEvents> calculateQueue(Wall walls[],Disc discs[],int totalDiscs,Disc striker);
//#define priority_queue<Event,vector<Event>,CompareEvents> (EventQueue)
#endif
