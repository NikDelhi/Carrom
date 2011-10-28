#include <iostream>
#include "physics.h"
#include "header.h"

using namespace std;

//Event :: Event(){}
Event :: Event(Disc& d1, Disc& d2) : disc1(d1),disc2(d2){}
Disc Event :: getDisc1()
{
	return disc1;
}
Disc Event :: getDisc2()
{
	return disc2;
}
double Event :: getTimeLeft()
{
	return timeLeft;
}
void Event :: setEvent(Disc& d1,Disc& d2,double t)
{
	disc1 = d1;
	disc2 = d2;
	timeLeft = t;		
}
void Event :: setDiscs(Disc& d1,Disc& d2)
{
	disc1 = d1;
	disc2 = d2;
}
void Event :: setTime(double t)
{
	timeLeft = t;
}
void Event :: calculateEvent(Disc& d1,Disc& d2)
{
	disc1 = d1;
	disc2 = d2;
	//timeLeft = calculateCollisionTime(d1,d2); 
}
void Event :: updateDiscs()
{
	//cout<<"start"<<endl;
	double vel[2][2];// = (double**) malloc(4*sizeof(double));
	afterCollision(disc1,disc2,vel);
	//cout<<vel[0][1];
	//cout<<"here"<<endl;
	disc1.updateDisc(vel[0]);
	disc2.updateDisc(vel[1]);
}

wallEvent :: wallEvent(Disc& d,Wall w) :disc(d){wall=w;}
Disc wallEvent :: getDisc()
{
return disc;
}
Wall wallEvent :: getWall()
{
return wall;
}
void wallEvent :: setEvent(Disc& d, Wall& w)
{
disc = d;
wall = w;
}
void wallEvent :: calculateEvent(Disc& d,Wall& w)
{
disc = d;
wall = w;
//timeLeft = calculateCollisionTime(d,w);
}
void wallEvent :: calculateEvent(Disc& d,Wall w[])
{
timeLeft = calculateEvent(d,w,0);
}
double wallEvent :: calculateEvent(Disc& d,Wall w[],int no)
{
double t;// = calculateCollisionTime(d,w[no]);
if(no == 4) return -1;
else if(t == -1) return calculateEvent(d,w,no+1);
else return t;
}
void wallEvent :: updateDiscs()
{
double *vel = (double*) malloc(2*sizeof(double));
vel = afterCollision(disc,wall);
//cout<<"vel should be updated to "<<vel[0]<<" "<<vel[1]<<endl;
disc.updateDisc(vel);
}

