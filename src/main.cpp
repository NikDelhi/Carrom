#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include "header.h"
#include "dispList.h"
#include "physics.h"

using namespace std;

double tmaxCoord = .5;
double tminCoord = -.5;
int windowMinCoord = -100;
int windowMaxCoord = 100;
//void calculateQueue(priority_queue<Event,vector<Event>,CompareEvents> queue);
/*
bool CompareEvents :: operator()(Event& e1,Event& e2)
{
	if(e1.getTimeLeft() < e2.getTimeLeft()) return true;
	return false;
}

void pushQueue(priority_queue<Event,vector<Event>,CompareEvents> q,Event v[],int len)
{
	for(int i = 0; i < len; i++)
	{
		q.push(v[i]);
	}
}
*/
void initializeDiscs(double m,Disc d[],int n,double r,double h,double X[],double Y[],int startNo)
{
	int i;
	for(i=0; i < n; i++)
	{
		d[i].createDisc(m,startNo+i,r,h,X[i],Y[i],0,0,0,0,0);
	}
}

void initializeHoles(Hole h[],double r)
{
	h[0].createHole(tminCoord+r,tminCoord+r,r);
	h[1].createHole(tmaxCoord-r,tminCoord+r,r);
	h[2].createHole(tmaxCoord-r,tmaxCoord-r,r);
	h[3].createHole(tminCoord+r,tmaxCoord-r,r);
}

void initializeWalls(Wall w[],Hole h[],double len,double wi,double hi)
{
	int i;
	for(i=0; i < 4; i++)
	{
		w[i].createWall(i,h[i],h[(i+1)==4?0:(i+1)],len,wi,hi,tminCoord,tmaxCoord,tmaxCoord,tminCoord);
	}
}

void printDiscs(Disc d[],int len)
{
	for(int i = 0; i < len; i++)
	{
		//cout<<d[i].getDiscNo()<<endl;
	}
}
/*
priority_queue<Event,vector<Event>,CompareEvents> calculateQueue(Wall walls[],Disc discs[],int totalDiscs,Disc striker)
{
	priority_queue<Event,vector<Event>,CompareEvents> eventQueue;
	//wallEvent wEvent = new wallEvent(striker,walls);
	//wEvent.calculateEvent(striker,walls);
	//eventQueue.push(wEvent);
	//cout<<"doing"<<endl;
	for(int i=0; i < totalDiscs; i++)
	{
		Event *ev = new Event(striker,discs[i]);
		//ev.calculateEvent(striker,discs[i]);
		//eventQueue.push(ev);
		//cout<<"doing"<<endl;
		//wallEvent wEvent;
		//wEvent.calculateEvent(discs[i],walls);
		//eventQueue.push(wEvent);
		//cout<<"doing"<<endl;
	}
	Event ev5 = eventQueue.top();
	//cout<<ev5.getTimeLeft()<<endl;
	//cout<<"doing end"<<endl;
	return eventQueue;
}
*/
int main(int argc,char** args)
{
	int totalDiscs = 6;
	double radius = .030;
	double strikerLow=tminCoord+.08,strikerHigh=tminCoord+.16,striker_radius=1.3*radius,hole_radius=1.3*radius;
	double stkRadius = striker_radius;
	//cout<<"stkradius"<<striker_radius<<"\n";
	double initx[] = {0,0,0,0,0,0};
	double inity[] = {-.10,.05,.150,.235,.310,.395};
	double stkx = 0;
	double stky = (strikerLow+strikerHigh)/2;
	//cout<<strikerHigh<<" "<<strikerLow<<endl;
	//sleep(4);
	double wallHeight = .10;
	double wallWidth = .15;
	double discHeight = .02;
	double coinMass = 2;
	double strikerMass = 2*coinMass;
	//friction=.3;
	//cout<<"masses "<<strikerMass<<" "<<coinMass<<endl;
	//priority_queue<Event,vector<Event>,CompareEvents> eventQueue;
	int i,j;
	Hole holes[4];
	Wall walls[4];
	Disc discs[totalDiscs];
	Disc striker;	
	
	initializeWalls(walls,holes,tmaxCoord-tminCoord,wallWidth,wallHeight);
	initializeDiscs(coinMass,discs,totalDiscs,radius,discHeight,initx,inity,1);
	striker.createDisc(strikerMass,0,stkRadius,discHeight,stkx,stky,0,0,0.0,.00,0);
	
	/*wallEvent wEvent;
	wEvent.calculateEvent(striker,walls);
	queue.push(wEvent);
	
	for(i=0; i < totalDiscs; i++)
	{
		Event ev;
		ev.calculateEvent(striker,discs[i]);
		eventQueue.push(ev);
	}*/
	//Display obj;
	//printDiscs(discs,totalDiscs);
	setTotalDiscs(totalDiscs);
	setminx(tminCoord);
	setminy(tminCoord);
	setmaxx(tmaxCoord);
	setmaxy(tmaxCoord);
	setDiscs(discs);
	setWalls(walls);
	setHoles(holes);
	setStriker(striker);
	setLinks(argc,args);
	initializeHoles(holes,hole_radius);
	startGame();
	
	return 0;
}
