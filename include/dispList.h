
#ifndef DISPLIST_H
#define DISPLIST_H
extern GLuint carrom;
extern double strikerLow,strikerHigh;
extern double strikerLow2,strikerHigh2;
extern double base_line_length;
extern double striker_radius;
extern double disc_radius;
extern double hole_radius;
void drawCarrom();
void disk(float R);
GLuint positionBalls(float R);
void arch(int rotate);
void semiCircle(int direction,float R);
void baseLines();
void arrows(float ratio_r);

#endif
