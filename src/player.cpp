#include <iostream>
#include <header.h>
#include <math.h>

#define fixed1 -.3
#define fixed2 .3
#define xnew(a,x,y,xc,yc) (xc + (x-xc)*cos(a) - (y-yc)*sin(a))
#define ynew(a,x,y,xc,yc) (yc + (y-yc)*cos(a) + (x-xc)*sin(a))
#define arrowLength .2

using namespace std;

void Player :: setPlayer(int no)
{
	playerno = no;
	strikerx = (no%2==0?0:(2-no)*fixed2);
	strikery = (no%2==1?0:(1-no)*fixed1);
	arrowx = strikerx + (no%2==0?0:(2-no)*arrowLength);
	arrowy = strikery - (no%2==1?0:(1-no)*arrowLength);
	lookx = (no%2==0?0:(no==1?1:-1));
	looky = (no%2==1?0:(no==0?-1:1));
	score = 0;
}

void Player :: moveLeft(double amt,double newCoords[])
{
	//cout<<"striker changing"<<endl;
	strikerx += (playerno%2==0?(playerno==0?-amt:amt):0);
	strikery += (playerno%2==1?(playerno==1?-amt:amt):0);
	newCoords[0] = strikerx;
	newCoords[1] = strikery;	
}

void Player :: moveArrowLeft(double angle,double newCoords[])
{
	//cout<<"arrow changing"<<endl;
	arrowx = xnew(-angle,arrowx,arrowy,strikerx,strikery);
	arrowy = ynew(-angle,arrowx,arrowy,strikerx,strikery);
	newCoords[0] = arrowx;
	newCoords[1] = arrowy;
}

void Player :: moveRight(double amt,double newCoords[])
{
	moveLeft(-amt,newCoords);	
}

void Player :: moveArrowRight(double angle,double newCoords[])
{
	moveArrowLeft(-angle,newCoords);
}

void Player :: setVelocity(double v[],double mulfactor)
{
	v[0] = strikerx-arrowx;
	v[1] = strikery-arrowy;
	double vm = sqrt(v[0]*v[0] + v[1]*v[1]);
	v[0] = mulfactor*v[0]/vm;
	v[1] = mulfactor*v[1]/vm;
	//cout<<"strikerx "<<strikerx<<" arrowx "<<arrowx<<endl;
	//cout<<"strikery "<<strikery<<" arrowy "<<arrowy<<endl;
	//cout<<"setting velocity "<<v[0]<<" "<<v[1]<<endl;
}

