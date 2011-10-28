#include <math.h>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "header.h"
#include "physics.h"

double error = 0;
double ew = 1;
double ed = 1;
double friction = 0.1;											//.05

bool isDiscInside(Disc d,Hole h)
{
	if(isLessThan(abs(d.getX() - h.getX()),h.getRadius()) && isLessThan(abs(d.getY() - h.getY()),h.getRadius()))
		return true;
	else
		return false;
}
/*
double calculateCollisionTime(Disc d,Wall w)
{
	
	return 0.3;
}
*/
double* afterCollision(Disc d,Wall w)
{
	double *v = (double*) malloc(2*sizeof(double));
	if(w.getPos()==1 || w.getPos() == 3) 
	{
		//cout<<"checking after collision "<<d.getVx();
		v[0] = d.getVx()*(-1*ew);
		//cout<<" "<<v[0];
		v[1] = d.getVy();
	} 
	else
	{
		v[1] = d.getVy()*(-1*ew);
		v[0] = d.getVx();
	}
	return v;
}

void afterCollision(Disc d1,Disc d2,double v[][2])
{
	//double **v = (double**) malloc(4*sizeof(double));
	//cout<<"array declared"<<endl;
	//sleep(2);
	double vector_par[2],vector_per[2],u_par[2],v_per[2],v_par[2],l;
	double v1[2][2],y,x;
	double v1x = d1.getVx(),v2x = d2.getVx(),v1y = d1.getVy(),v2y = d2.getVy(),m1 = d1.getMass(),m2 = d2.getMass();
	double slope;
	y=(d1.getY()-d2.getY());
	x=(d1.getX()-d2.getX());
	l=sqrt(y*y+x*x);
	vector_par[0]=x/l;
	vector_par[1]=y/l;
	vector_per[0]=y/l;
	vector_per[1]=-x/l;
	//cout<<"vector par"<<vector_par[0]<<"+"<<vector_par[1]<<"\n";
	//cout<<"vector_per"<<vector_per[0]<<"+"<<vector_per[1]<<"\n";
	//cout<<"v1"<<v1x<<"+"<<v1y<<"\n";
	//cout<<"v2"<<v2x<<"+"<<v2y<<"\n";
	u_par[0]=v1x*vector_par[0]+v1y*vector_par[1];
	u_par[1]=v2x*vector_par[0]+v2y*vector_par[1];
	//cout<<u_par[0]<<"+"<<u_par[1]<<"\n";
	
	v_per[0]=v1x*vector_per[0]+v1y*vector_per[1];
	v_per[1]=v2x*vector_per[0]+v2y*vector_per[1];
		//cout<<v_per[0]<<"+"<<v_per[1]<<"\n";

	v_par[0]=(m1*u_par[0] + m2*u_par[1] + m2*ed*(u_par[1]-u_par[0]))/(m1+m2);
	v_par[1]=(m1*u_par[0] + m2*u_par[1] - m1*ed*(u_par[1]-u_par[0]))/(m1+m2);
	//cout<<v_par[0]<<"+"<<v_par[1]<<"+";
	
	//cout<<"slope"<<slope<<"\n";
	//sleep(2);
	v[0][0]=v_par[0]*vector_par[0]+v_per[0]*vector_per[0];
	v[0][1]=v_par[0]*vector_par[1]+v_per[0]*vector_per[1];
	v[1][0]=v_par[1]*vector_par[0]+v_per[1]*vector_per[0];
	v[1][1]=v_par[1]*vector_par[1]+v_per[1]*vector_per[1];
	//cout<<"disk"<<v[0][0]<<"+"<<v[0][1]<<"\n";
	//cout<<"striker"<<v[1][0]<<"+"<<v[1][1]<<"\n";
	
	
}

bool checkNextCollision(Disc d1,Disc d2)
{
	double relVx = d1.getVx() - d2.getVx();
	double relVy = d1.getVy() - d2.getVy();
	double relx = d1.getX() - d2.getX();
	double rely = d1.getY() - d2.getY();
	if(relVx*relx + relVy*rely < 0) return true;
	return false;
}

bool checkNearness(Disc d1,Disc d2)
{
	double vrel_radial,arel_radial;
	double vector_par[2],x,y,l;
	double xgap = abs(d1.getX() - d2.getX());
	double ygap = abs(d1.getY() - d2.getY());
	y=(d1.getY()-d2.getY());
	x=(d1.getX()-d2.getX());
	l=sqrt(y*y+x*x);
	
	vector_par[0]=x/l;
	vector_par[1]=y/l;
	vrel_radial=(d1.getVx()-d2.getVx())*vector_par[0]+(d1.getVy()-d2.getVy())*vector_par[1];
	arel_radial=(d1.getAx()-d2.getAx())*vector_par[0]+(d1.getAy()-d2.getAy())*vector_par[1];
	error=vrel_radial*delt+0.5*arel_radial*delt*delt;
	double r = d1.getRadius() + d2.getRadius()+error;
	if(xgap*xgap + ygap*ygap<=r*r )
	//sleep(1);
	if(xgap*xgap + ygap*ygap <= r*r && (checkNextCollision(d1,d2)))
	{
		cout<<"colission bw "<<d1.getDiscNo()<<" "<<d2.getDiscNo()<<endl;
		//sleep(1);
		error=0;
		return true;
		
	}
	error=0;
	return false;
}

bool checkNearness(Disc d, Wall w,int wallno)
{
	bool cond = (wallno==0 || wallno==2);
	if(abs((cond?d.getY():d.getX()) - (cond?w.getY():w.getX())) <= d.getRadius() && (cond?d.getVy():d.getVx())*(cond?w.getY():w.getX())>= 0)
	{
		//cout<<d.getDiscNo()<<" colliding with wall"<<wallno<<endl;
		return true;
	}
	return false;
}

int checkNearness(Disc d,Wall w[])
{
	for(int i = 0; i < 4; i++)
	{
		if(checkNearness(d,w[i],w[i].getPos())) return w[i].getPos();
	}
	return -1;
}

int checkDiscCollisions(Disc d[],Wall w[],int n,Disc stk,int ret[][2])
{
	int reti=0;
	int wallNo;
	for(int i = 0; i < n; i++)
	{
		if(checkNearness(d[i],stk))
		{
			//cout<<"colliding disc and striker "<<i<<" "<<endl;
			ret[reti][0] = i;
			ret[reti++][1] = -1;
		}
		if((wallNo = checkNearness(d[i],w)) != -1) 
		{
			ret[reti][0] = i;
			ret[reti++][1] = wallNo-5;
			//cout<<"colliding wall"<<i<<" "<<wallNo<<endl;
		}
		for(int j = i+1; j < n; j++)
		{
			//cout<<"colliding disc"<<i<<" "<<j<<endl;
			if(checkNearness(d[i],d[j]))
			{
				ret[reti][0] = i;
				ret[reti++][1] = j;
			}
		
		}
	}
	if((wallNo = checkNearness(stk,w)) != -1)
	{
		//cout<<"colliding striker with wall"<<" "<<wallNo<<endl;
		ret[reti][0] = -1;
		ret[reti++][1] = wallNo-5;
	}
	return reti;

}
