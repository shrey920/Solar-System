#include<windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
using namespace std;
class vertex {
public:
    double x;
    double y;
    double z;
    vertex(double x,double y, double z=0){
        this->x=x;
        this->y=y;
        this->z=z;
    }
};

void scanline(double h)
{
 int varx;
 for(int i=0;i<c.size();i++)
 for(varx=h+c[i].x ; varx>=(h-c[i].x) ; varx--)
 glVertex3s(varx,c[i].y,c[i].z);
}
vertex circle(double h,double k,double r)
{
 vector<vertex> c;
 double XEnd,J;
 double i,j;
 XEnd=(r/1.414);
 for( i=0 ; i<=XEnd ; i=i+0.01)
 {
 J=sqrt(r*r - i*i);
 j=J;
 vertex v = vertex(i,j,0);
 c.push_back(v);
 v = vertex(j,i,0);
 c.push_back(v);
 v = vertex(j,-i,0);
 c.push_back(v);
 v = vertex(i,-j,0);
 c.push_back(v);
 }
 glVertex3s(h,k,-25);
 scanline(h,c);
 return c;
}

 void display(void)
 {
 glClear (GL_COLOR_BUFFER_BIT);

 glColor3f (1.0, 0.0, 0.0);

 glBegin(GL_POINTS);
 vector<vertex> c = circle(0,0,50);
 glEnd();
 glFlush();
 }


 void init(void)
 {
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
 }
int main(int argc, char** argv)
{
 glutInit(&argc, argv);
 glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
 glutInitWindowSize (500, 500);
 glutInitWindowPosition (100, 100);
 glutCreateWindow ("Circle : Scanline Filling Method ");
 init ();
 glutDisplayFunc(display);
 glutMainLoop();
 return 0;
}
