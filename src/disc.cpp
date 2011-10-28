#include <iostream>
#include "header.h"
#include "physics.h"

using namespace std;


int Disc :: getDiscNo()
{
	return discNo;
} 
double Disc :: getMass()
{
	return m;
}
double Disc :: getRadius()
{
	return radius;
} 
double Disc :: getHeight()
{
	return height;
} 
double Disc :: getX()
{
	return x;
} 
double Disc :: getY()
{
	return y;
} 
double Disc :: getZ()
{
	return z;
} 
double Disc :: getVx()
{
	return Vx;
} 
double Disc :: getVy()
{
	return Vy;
} 
double Disc :: getVz()
{
	return Vz;
} 
double Disc :: getAx()
{
	return Ax;
} 
double Disc :: getAy()
{
	return Ay;
} 
double Disc :: getAz()
{
	return Az;
} 
void Disc :: setAx(double a)
{
	Ax = a;
}
void Disc :: setAy(double a)
{
	Ay = a;
}
void Disc :: updateDisc(double v[])
{
	//cout<<"updating disc with v "<<v[0]<<" "<<v[1]<<endl;
	//cout<<v[0]<<endl;;
	Vx = v[0];
	Vy = v[1];
}
void Disc :: updateDisc(double vx,double vy)
{
	//cout<<"updating disc with v "<<v[0]<<" "<<v[1]<<endl;
	//cout<<v[0]<<endl;;
	Vx = vx;
	Vy = vy;
}
void Disc :: updateCoords(double X,double Y)
{
	x = X;
	y = Y;
}
void Disc :: updateCoords(double a[])
{
	x = a[0];
	y = a[1];
}
void Disc :: updateDisc(double X,double Y,double velx,double vely,double accx,double accy,double time)
{
	x = X;
	y = Y;
	z = 0;
	Vx = velx;
	Vy = vely;
	Vy = 0;
	Ax = accx;
	Ay = accy;
	Az = 0;
	latestUpdateTime = time;
}
void Disc :: createDisc(double mass,int n,double r,double h,double X,double Y,double velx,double vely,double accx,double accy,double time)
{
	m = mass;
	discNo = n;
	radius = r;
	height = h;
	x = X;
	y = Y;
	Vx = velx;
	Vy = vely;
	Ax = accx;
	Ay = accy;
	latestUpdateTime = time;
}
void Disc :: createDisc(Disc d)
{
	//cout<<"disc coords "<<d.getX()<<" "<<d.getY();
	m = d.getMass();
	discNo = d.getDiscNo();
	radius = d.getRadius();
	height = d.getHeight();
	x = d.getX();
	y = d.getY();
	Vx = d.getVx();
	Vy = d.getVy();
	Ax = d.getAx();
	Ay = d.getAy();
	latestUpdateTime = d.getTime();
}
void Disc :: setTime(double time)
{
	latestUpdateTime = time;		
}
double Disc :: getTime()
{
	return latestUpdateTime;
}
void Disc :: updateDisc(double time)
{
	/*double* ret = (double*)malloc(sizeof(double)*2);
	double t = time;// - latestUpdateTime;
	//cout<<Ax<<" "<<Ay<<endl;
	double fx,fy;
	if((-error<=Vx || Vx<=error) && (-error<=Vy || Vy<=error)) 
	{
		//Vx = 0;
		//Vy = 0;
		fx = 0;
		fy = 0;
	}
	else
	{
		fx = Vx/(sqrt(Vx*Vx+Vy*Vy))*friction;
		fy = Vy/(sqrt(Vx*Vx+Vy*Vy))*friction;
	}*/
	//double* ret = (double*)malloc(sizeof(double)*2);
	double t = time;// - latestUpdateTime;
	//cout<<Ax<<" "<<Ay<<endl;
	Ax=Ay=0;
	double fx=0,fy=0;
	double vx_next=0,vy_next=0;
	/*if(Vx<error && Vx>-error)
	{
		Vx = 0;
		fx = 0;
	}
	if(Vy<error && Vy>-error)
	{
		Vy = 0;
		fy = 0;
	}*/
	if(Vx!=0)	
	{
		fx = Vx/(sqrt(Vx*Vx+Vy*Vy))*friction;
		vx_next =(Vx-(fx)*t)/Vx;
	}
	if(Vy!=0)
	{
		fy = Vy/(sqrt(Vx*Vx+Vy*Vy))*friction;
		vy_next =(Vy-(fy)*t)/Vy;
		
	}
	//return;
	//printf("vx_next %d\n",)
	//sleep(1);
	if(vx_next<=0) 
	{
		Vx = 0;
		fx = 0;
	}
	if(vy_next<=0)
	{
		Vy=0;
		fy=0;
		
	}
	//if(fx*Vx > 0) fx = -fx;
	//else if(fy*Vy > 0) fy = -fy;
	if(vx_next>0 )
	{
		fx = Vx/(sqrt(Vx*Vx+Vy*Vy))*friction;
		
		Vx =Vx-fx*t;
	
	}
	if(vy_next>0)
	{
		fy = Vy/(sqrt(Vx*Vx+Vy*Vy))*friction;
		Vy =Vy-fy*t;
	}
	Ax -= fx;
	Ay -= fy;
	x += (Vx)*t + .5*(Ax)*t*t;
	y += (Vy)*t + .5*(Ay)*t*t;
	//cout<<"vx "<<Vx<<"+"<<"vy "<<Vy<<"\n";
	//sleep(1);
	//x+=ret[0];
	//y+=ret[1];
	//latestUpdateTime += time;
	return ;
}
void Disc :: updateDisc()
{
	x += Vx;
	y += Vy;
}
void Disc :: setRadius(float r)
{
	radius=r;
}
bool Disc :: isAtRest()
{
	if(Vx!=0 || Vy!=0 || Ax!=0 || Ay!=0) return false;
	return true;
}
		
/*bool Disc :: checkFall(Hole holes[])
{
	for(int i = 0; i < 4; i++)
	{
		if(holes[i].isDiscIn(this)) return true;
	}
	return false;
}*/

