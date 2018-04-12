#include<windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
using namespace std;
class Circle {
public:
    double x;
    double y;
    double z;
    double r;
    Circle(double x,double y, double z, double r){
        this->x=x;
        this->y=y;
        this->z=z;
        this->r=r;
    }
};

void scanline(double h, double k, double x, double y, double z)
{
 float varx;
 for(varx=h+x ; varx>=(h-x) ; varx-=0.01)
 glVertex3f(varx,y,z);
}
void circle(double x, double y, double z, double r)
{
 double XEnd,J;
 double i,j;
 XEnd=(r/1.414);
 for( i=0 ; i<=XEnd ; i=i+0.01)
 {
 J=sqrt(r*r - i*i);
 j=J;
 scanline(x,y,i,j,z);
 scanline(x,y,i,-j,z);
 scanline(x,y,j,i,z);
 scanline(x,y,j,-i,z);
 }
 glVertex3f(x,y,z);
}

 void display(void)
 {
 glClear (GL_COLOR_BUFFER_BIT);

 glColor3f (1.0, 0.0, 0.0);

 glBegin(GL_POINTS);
 Circle c = Circle(0,0,0,1);
 circle(c.x,c.y,c.z,c.r);
 glEnd();
 glFlush();
 glutSwapBuffers();
 }


 void init(void)
 {
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
 }
int main(int argc, char** argv)
{
 glutInit(&argc, argv);
 glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
 glutInitWindowSize (500, 500);
 glutInitWindowPosition (100, 100);
 glutCreateWindow ("Circle : Scanline Filling Method ");
 init ();
 glutDisplayFunc(display);
 glutMainLoop();
 return 0;
}
