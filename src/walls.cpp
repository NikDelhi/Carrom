#include <iostream>
#include "header.h"

using namespace std;


int Wall :: getPos()
{
	return no;
}
Hole Wall :: getHole1()
{
	return hole1;
}
Hole Wall :: getHole2()
{
	return hole2;
}
double Wall :: getLength()
{
	return length;
}
double Wall :: getWidth()
{
	return width;
}
double Wall :: getHeight()
{
	return height;
}
double Wall :: getX()
{
	return x;
}
double Wall :: getY()
{
	return y;
}
double Wall :: getZ()
{
	return z;
}
void Wall :: setZ(double c)
{
	z = c;
}
void Wall :: createWall(int p,Hole h1,Hole h2,double len,double w,double h,double c1,double c2,double c3,double c4)
{
	no = p;
	hole1 = h1;
	hole2 = h2;
	length = len;
	width = w;
	height = h;
	if(p==0 || p==2)
	{
		x = -1;
		y = (p?c3:c1);
	}
	else
	{
		y = -1;
		x = (p==1?c2:c4);
	}
}


