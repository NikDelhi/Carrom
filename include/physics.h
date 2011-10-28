
#ifndef PHYSICS_H
#define PHYSICS_H
#include "header.h"
#include <math.h>
extern double friction;
extern double error;
extern double delt;
#define isLessThan(x,y) x<y+error
#define isMoreThan(x,y) x>y+error

//double calculateCollisionTime(Disc ,Disc );
//double calculateCollisionTime(Disc ,Wall );
bool isDiscInside(Disc ,Hole );
double* afterCollision(Disc ,Wall );
void afterCollision(Disc ,Disc ,double [][2]);
bool checkNearness(Disc d1,Disc d2);
int checkDiscCollisions(Disc d[],Wall w[],int n,Disc stk,int ret[][2]);

#endif

