#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include "header.h"
#include "dispList.h"

#define window_size 700.0
#define board_size 400.0 
#define scale_pixels board_size/74.0
#define corr_ratio 1/2
#define len_ratio 1
#define up 2
#define down 4
#define right 1
#define left 3
#define PI 3.1415926535898

GLuint balls;
GLuint carrom;
double base_line_length;
float speed1=0,speed2=0,x=0,y=0,t=0,x1=0,x2=0,size=20;
float corr_x1=-.1,corr_y1=0.1,corr_x2=.1,corr_y2=0.1;
double strikerLow,strikerHigh,strikerLow2,strikerHigh2,striker_radius,disc_radius,hole_radius;

/*GLfloat black[] = {0.0,0.0,0.0};
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
GLfloat white[] = {1.0,1.0,1.0};*/

void disk(float R)
{
	float l,x1,y1;
	int count;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0,0);
	for(count=0;count<=200;count++)
	{
		l=1.8*count/180*3.143;
		x1=(R)*len_ratio*cos(l);
		y1=(R)*len_ratio*sin(l);
		glVertex3f(x1,y1,0);
	}
	glEnd();
	

}
GLuint positionBalls(float R)
{
	GLuint list;
	 list=glGenLists(1);
	glNewList(list, GL_COMPILE);
	disk(R);
	glEndList();
	return list;
}
/*
void mouse(int button, int state, int x, int y)
{
switch (button) {
case GLUT_LEFT_BUTTON:
speed1=(speed1>0)?speed1+0.1:speed1-0.1;
speed2=(speed2>0)?speed2+0.1:speed2-0.1;
t=.00001;
glutPostRedisplay();
/*if (state == GLUT_DOWN)
glutIdleFunc(spinDisplay);
break;
case GLUT_MIDDLE_BUTTON:
if (state == GLUT_DOWN)
glutIdleFunc(NULL);
break;
default:
break;
}
}
void keyInput(unsigned char key,int x,int y)
{
	switch(key)
	{
		case ' ':
		size++;
		glutPostRedisplay();
		break;
		case 27:
		exit(1);
	}
}*/
void arch(int rotate)
{

	glColor3f(0,0,0);
	int count;
	float x1,y1,l;
	float R=3.15*scale_pixels;
	glBegin(GL_LINE_STRIP);
	glLineWidth(.5*scale_pixels);
	for(count=-41;count<=91;count++)
	{
		l=(1.8*count+90.0*(float)rotate)/180*3.143;
		x1=(R)*len_ratio*cos(l);
		y1=(R)*len_ratio*sin(l);
		glVertex3f(x1,y1,0);
	}
	glEnd();	


}
void semiCircle(int direction,float R)			
{
	glColor3f(0,0,0);
	int count;
	float x1,y1,l;
	
	glBegin(GL_LINE_STRIP);
	glLineWidth(.5*scale_pixels);
	for(count=0;count<=100;count++)
	{
		l=(1.8*count+90.0*(float)direction)/180*3.143;
		x1=(R)*len_ratio*cos(l);
		y1=(R)*len_ratio*sin(l);
		glVertex3f(x1,y1,0);
	}
	glEnd();


}
void baseLines()
{
	float R=3.04*scale_pixels/2;
	float offsetx,offsety,thickness;
	thickness=3.04*scale_pixels*len_ratio;
	striker_radius=2.09*scale_pixels*len_ratio;
	
	offsetx=23.1*scale_pixels*len_ratio;
	base_line_length=2*offsetx;
	offsety=9.4*scale_pixels*len_ratio;
	glLineWidth(.5*scale_pixels);
	glBegin(GL_LINES);
	glVertex2f(-offsetx,strikerLow = (-board_size*corr_ratio+offsety));
	glVertex2f( offsetx,-board_size*corr_ratio+offsety);
	glVertex2f(-offsetx,strikerHigh2 = (board_size*corr_ratio-offsety));
	glVertex2f( offsetx,board_size*corr_ratio-offsety);
	glVertex2f(-offsetx,strikerHigh = (-board_size*corr_ratio+offsety+thickness));
	glVertex2f(offsetx,-board_size*corr_ratio+offsety+thickness);
	glVertex2f(-offsetx,strikerLow2 = (board_size*corr_ratio-offsety-thickness));
	glVertex2f(offsetx,board_size*corr_ratio-offsety-thickness);
	glVertex2f(-board_size*corr_ratio+offsety,-offsetx);
	glVertex2f(-board_size*corr_ratio+offsety,offsetx);
	glVertex2f(board_size*corr_ratio-offsety,-offsetx);
	glVertex2f(board_size*corr_ratio-offsety,offsetx);
	glVertex2f(-board_size*corr_ratio+offsety+thickness,-offsetx);
	glVertex2f(-board_size*corr_ratio+offsety+thickness,offsetx);
	glVertex2f(board_size*corr_ratio-offsety-thickness,-offsetx);
	glVertex2f(board_size*corr_ratio-offsety-thickness,offsetx);
	
	glEnd();
	glTranslatef(-offsetx,-board_size*corr_ratio+offsety+thickness/2,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(right,R);
	glLoadIdentity();
	glTranslatef(offsetx,-board_size*corr_ratio+offsety+thickness/2,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(left,R);
	glLoadIdentity();
	glTranslatef(-offsetx,board_size*corr_ratio-offsety-thickness/2,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(right,R);
	glLoadIdentity();
	glTranslatef(offsetx,board_size*corr_ratio-offsety-thickness/2,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(left,R);
	glLoadIdentity();
	glTranslatef(-board_size*corr_ratio+offsety+thickness/2,-offsetx,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(up,R);
	glLoadIdentity();
	glTranslatef(-board_size*corr_ratio+offsety+thickness/2,offsetx,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(down,R);
	glLoadIdentity();
	glTranslatef(board_size*corr_ratio-offsety-thickness/2,-offsetx,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(up,R);
	glLoadIdentity();
	glTranslatef(board_size*corr_ratio-offsety-thickness/2,offsetx,0);
	glColor3f(1,0,0);
	disk(1.27*scale_pixels);
	glColor3f(0,0,0);
	semiCircle(down,R);
	glLoadIdentity();
	
	glColor3f(0,0,0);
	glTranslatef(-board_size*corr_ratio+offsety+thickness/2+.01,-board_size*corr_ratio+.01+offsety+thickness/2,0);
	semiCircle(up,(thickness-.02)/2*board_size);
	semiCircle(down,(thickness-.02)/2*board_size);
	glLoadIdentity();
	glTranslatef(board_size*corr_ratio-offsety-thickness/2-.01,-board_size*corr_ratio+.01+offsety+thickness/2,0);
	semiCircle(up,(thickness-.02)/2*board_size);
	semiCircle(down,(thickness-.02)/2*board_size);
	glLoadIdentity();
	glTranslatef(-board_size*corr_ratio+offsety+thickness/2+.01,board_size*corr_ratio-.01-offsety-thickness/2,0);
	semiCircle(up,(thickness-.02)/2*board_size);
	semiCircle(down,(thickness-.02)/2*board_size);
	glLoadIdentity();
	glTranslatef(board_size*corr_ratio-offsety-thickness/2-.01,board_size*corr_ratio-.01-offsety-thickness/2,0);
	semiCircle(up,(thickness-.02)/2*board_size);
	semiCircle(down,(thickness-.02)/2*board_size);
	glLoadIdentity();
}

void arrows(float ratio_r)
{
	glColor3f(0,0,0);
	glBegin(GL_LINES);
	x=-board_size*corr_ratio+ratio_r+(ratio_r+5.0*scale_pixels*len_ratio)*cos(PI/4);
	y=-board_size*corr_ratio+ratio_r+(ratio_r+5.0*scale_pixels*len_ratio)*sin(PI/4);
	glVertex2f(x,y);
	x=x+25.0*scale_pixels*len_ratio*cos(PI/4);
	y=y+25.0*scale_pixels*len_ratio*sin(PI/4);	
	glVertex2f(x,y);
	glEnd();
	x=x-3.15*scale_pixels*len_ratio*cos(PI/4);
	y=y-3.15*scale_pixels*len_ratio*sin(PI/4);
	glTranslatef(x,y,0);
	arch(0);
	glLoadIdentity();
	
	 
	 glBegin(GL_LINES);
	x=-board_size*corr_ratio+ratio_r+(ratio_r+5.0*scale_pixels*len_ratio)*cos(PI/4);
	y=board_size*corr_ratio-ratio_r-(ratio_r+5.0*scale_pixels*len_ratio)*sin(PI/4);
	glVertex2f(x,y);
	x=x+25.0*scale_pixels*len_ratio*cos(PI/4);
	y=y-25.0*scale_pixels*len_ratio*sin(PI/4);	
	glVertex2f(x,y);
	glEnd();
	x=x-3.15*scale_pixels*len_ratio*cos(PI/4);
	y=y+3.15*scale_pixels*len_ratio*sin(PI/4);
	glTranslatef(x,y,0);
	arch(3);
	glLoadIdentity();
	
	 
	 glBegin(GL_LINES);
	x=board_size*corr_ratio-ratio_r-(ratio_r+5.0*scale_pixels*len_ratio)*cos(PI/4);
	y=board_size*corr_ratio-ratio_r-(ratio_r+5.0*scale_pixels*len_ratio)*sin(PI/4);
	glVertex2f(x,y);
	x=x-25.0*scale_pixels*len_ratio*cos(PI/4);
	y=y-25.0*scale_pixels*len_ratio*sin(PI/4);	
	glVertex2f(x,y);
	glEnd();
	x=x+3.15*scale_pixels*len_ratio*cos(PI/4);
	y=y+3.15*scale_pixels*len_ratio*sin(PI/4);
	glTranslatef(x,y,0);
	arch(2);
	glLoadIdentity();
	
	 
	 glBegin(GL_LINES);
	x=board_size*corr_ratio-ratio_r-(ratio_r+5.0*scale_pixels*len_ratio)*cos(PI/4);
	y=-board_size*corr_ratio+ratio_r+(ratio_r+5.0*scale_pixels*len_ratio)*sin(PI/4);
	glVertex2f(x,y);
	x=x-25.0*scale_pixels*len_ratio*cos(PI/4);
	y=y+25.0*scale_pixels*len_ratio*sin(PI/4);	
	glVertex2f(x,y);
	glEnd();
	x=x+3.15*scale_pixels*len_ratio*cos(PI/4);
	y=y-3.15*scale_pixels*len_ratio*sin(PI/4);
	glTranslatef(x,y,0);
	arch(1);
	glLoadIdentity();
	
	 
	



}
void drawCarrom()
{
	float x,y;
	float radius=2.22*scale_pixels;		//in pixels
	float ratio_r=radius*len_ratio	;	// r in coordinate scale
	disc_radius=1.56*scale_pixels*len_ratio;
	hole_radius=ratio_r;
	disc_radius=1.54*scale_pixels*len_ratio;
	 carrom=glGenLists(1);
	glNewList(carrom, GL_COMPILE);
	glColor3fv(brown);
	glRectf(-board_size*corr_ratio,-board_size*corr_ratio,board_size*corr_ratio,board_size*corr_ratio);
	glColor3f(0,0,0);										////////////////HOLES///////
	glTranslatef(-board_size*corr_ratio+ratio_r,-board_size*corr_ratio+ratio_r,0);
	disk(radius);
	glLoadIdentity();
	glTranslatef(board_size*corr_ratio-ratio_r,-board_size*corr_ratio+ratio_r,0);
	disk(radius);
	glLoadIdentity();
	glTranslatef(board_size*corr_ratio-ratio_r,board_size*corr_ratio-ratio_r,0);
	disk(radius);
	glLoadIdentity();
	glTranslatef(-board_size*corr_ratio+ratio_r,board_size*corr_ratio-ratio_r,0);
	disk(radius);
	glLoadIdentity();
	baseLines();
	
	glColor3f(1,0,0);
	disk(1.59*scale_pixels);
		glColor3f(0,0,0);
	semiCircle(up,1.59*scale_pixels);
	semiCircle(down,1.59*scale_pixels);
	
	glColor3f(0,0,0);
	semiCircle(up,8.5*scale_pixels);
	semiCircle(down,8.5*scale_pixels);

	arrows(ratio_r);
	glEndList();
	
}
/*
static void init(void)
{

glShadeModel(GL_FLAT);
glClearColor(0.0, 0.0, 0.0, 0.0);
glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
balls=positionBalls(20);
glLoadIdentity();

drawCarrom();
}
void display(void)
{

	glCallList(carrom);
glTranslatef(.3,.4,0);

glLoadIdentity();
corr_x1=corr_x1+speed1*.001;
	if(corr_x1>=board_size*corr_ratio-size*len_ratio||corr_x1<=-board_size*corr_ratio+size*len_ratio)
	speed1=-speed1;
	corr_x2=corr_x2+speed2*.001;
		if(corr_x2+x2>=board_size*corr_ratio-size*len_ratio||corr_x2+x2<=-board_size*corr_ratio+size*len_ratio)
	speed2=-speed2;
glTranslatef(corr_x1,corr_y1,0);	
glColor3f(1,0,1);
glCallList(balls);
glLoadIdentity();
glTranslatef(corr_x2,corr_y2,0);	
glColor3f(1,0,0);
glCallList(balls);
glLoadIdentity();
glutPostRedisplay();
glutSwapBuffers();
}

int main(int argc, char **argv)
{
glutInitWindowSize(window_size, window_size);
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
glutCreateWindow(argv[0]);
init();
//glutReshapeFunc(reshape);
glutKeyboardFunc(keyInput);
glutMouseFunc(mouse);
glutDisplayFunc(display);
glutMainLoop();
return 0;
}
*/
