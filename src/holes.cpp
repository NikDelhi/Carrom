#include <iostream>
#include <math.h>
#include "physics.h"
#include "header.h"

using namespace std;


double Hole :: getX()
{
	return x;
}
double Hole :: getY()
{
	return y;
}
double Hole :: getRadius()
{
	return radius;
}
void Hole :: createHole(double X,double Y,double R)
{
	x = X;
	y = Y;
	radius = R;
}
/*bool Hole :: isDiscIn(Disc d)
{
	return isDiscInside(d,this);
}*/

