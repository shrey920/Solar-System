#include <cmath>
#ifdef __SUN__
#include <GLUT/glut.h>
#else
#include<windows.h>
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <cstdlib>
void *currentfont;


void drawstring(float x,float y,float z,unsigned char *string){
	unsigned char *c;
	glutBitmapLength(currentfont, string);
	glRasterPos3f(x,y,z);
	for(c=string;*c!='\0';c++)
	{
		glutBitmapCharacter(currentfont,*c);
	}
}

void setFont(void *font){
		currentfont=font;
}
/* GLUT callback Handlers */

void drawLine(double x1,double y1,double x2,double y2)
{
    if(abs(x1-x2)<0.1){
        if(y1>y2){
            double temp=y1;
            y1=y2;
            y2=temp;
        }
        for(double y=y1;y<y2;y=y+0.1){
            glBegin(GL_POINTS);
              glVertex3f(x1,y,-10);
            glEnd();
        }

    }
    if(x1>x2){
        double temp=x1;
        x1=x2;
        x2=temp;
        temp=y1;
        y1=y2;
        y2=temp;
    }
   double m = (y2 - y1)/(x2 - x1);
   for (double x  = x1; x <= x2; x=x+0.1)
   {
      // Assuming that the round function finds
      // closest integer to a given float.
      double y = m*x;
      glBegin(GL_POINTS);
      glVertex3f(x,y,-10);
      glEnd();
   }
}

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
 J=sqrt(r*r - i*i );
 j=J;
 scanline(x,y,i,y+j,z);
 scanline(x,y,i,y-j,z);
 scanline(x,y,j,y+i,z);
 scanline(x,y,j,y-i,z);
 }
 glVertex3f(x,y,z);
}

void initopenGL()
{
    glClearColor(0, 0, 0, 0);
    glColor3f(1,0,0);
    gluOrtho2D(-100,100,-100,100);

}

static void resize(int width, int height)
{
	const float ar = (float) width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
}

// Planet class that will hold all our planetary data
class Planet {
public:
	std::string name = "";
	double x = 0;
	double y = 0;
	double c1 = 0;
	double c2 = 0;
	double c3 = 0;
	double f = 0;
	double a = 0;
	double s1 = 0;

	Planet(std::string,double,double,double,double,double,double);
	void glStuff(double t, double closeness);
	//void printPlanet();
};


// Simple planet constructor
Planet::Planet(std::string name="",double c1=0,double c2=0,double c3=0,double f=0,double a=0,double s1=0) {
	this->name = name;
	this->x = 0;
	this->y = 0;
	this->c1 = c1;
	this->c2 = c2;
	this->c3 = c3;
	this->f = f;
	this->a = a;
	this->s1 = s1;
}
double moon_x; // moon x position
double moon_y; // moon y position
double moon_r=0.1;

// Function will compute new planet position
void Planet::glStuff(double t, double closeness) {
	this->x = sin(t*f)*a*1.5/abs(closeness) * 10;
	this->y = cos(t*f)*a/abs(closeness) * 10;
    double r = this->s1/abs(closeness) * 10;

    glColor3f(c1,c2,c3);
    glBegin(GL_POINTS);
        circle(this->x,this->y,-10,r);
    glEnd();

}

static double closeness = -20.0;
Planet** planets;
static double t; // time
double tinc=0.001;

static void display(void) // void
{
    //t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 9; i++) {
		(*planets[i]).glStuff(t, closeness);
	}

	// moon orbit calculation
	moon_x = planets[3]->x + (-sin(t)*0.5)/abs(closeness) * 10;;
	moon_y = planets[3]->y + (cos(t)*0.5)/abs(closeness) * 10;;

    float e=sqrt(planets[3]->x*planets[3]->x + planets[3]->y*planets[3]->y);
    float m=sqrt(moon_x*moon_x + moon_y*moon_y);
    float d= sqrt((moon_x-planets[3]->x)*(moon_x-planets[3]->x) +(moon_y-planets[3]->y)*(moon_y-planets[3]->y));


    if(closeness>-15.0 && m-e+d < 0.00001 ){
        setFont(GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0,1.0,1.0);
        unsigned char string[]="SOLAR ECLIPSE.";
        drawstring(-1.2,3.0,-10.0,string);
        drawLine(planets[0]->x,planets[0]->y, planets[3]->x,planets[3]->y);
        for(int i=0;i<10000;i++) for(int j=0;j<5000;j++){}
    }

    if(closeness>-15.0 && e-m+d < 0.00001 ){
        setFont(GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0,1.0,1.0);
        unsigned char string[]="LUNAR ECLIPSE.";
        drawstring(-1.2,3.0,-10.0,string);
        drawLine(planets[0]->x,planets[0]->y, moon_x,moon_y);
        for(int i=0;i<10000;i++) for(int j=0;j<5000;j++){}
    }



    // "Moon"
    moon_r=0.1/abs(closeness) * 10;
	glColor3d(0.7,0.7,0.7);
	glBegin(GL_POINTS);
        circle(moon_x,moon_y,-10,moon_r);
    glEnd();
    t=t+tinc;
	glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27 :
		case 'q':
			exit(0);
			break;
		case '+':
			if (closeness < -4.0) {
				closeness += 0.5;
			}
			break;
		case '-':
			closeness -= 0.5;
			break;
        case '*':
            tinc = tinc + 0.001;
            break;
        case '/':
            tinc = tinc - 0.001;
            break;
	}
	glutPostRedisplay();
}

static void idle(void)
{
	glutPostRedisplay();
}




int main(int argc, char *argv[])
{
	// Although the sun is not a planet it is
	// convenient to keep in our array of planets
	planets = new Planet*[9];
	Planet q("sun", 1.0, 0.72, 0.07, 0.0, 0.0, 1.5);        planets[0] = &q;
	Planet a("mercury", 0.5, 0.4, 0.4, 1.0, 2.0, 0.15);   planets[1] = &a;
	Planet s("venus", 0.6, 0.6, 0.2, 0.5, 3.0, 0.18);     planets[2] = &s;
	Planet d("earth", 0.0, 0.0, 0.7, 0.2, 4.0, 0.2);      planets[3] = &d;
	Planet f("mars", 1.0, 0.2, 0.0, 0.15, 5.5, 0.15);     planets[4] = &f;
	Planet z("jupiter", 0.8, 0.6, 0.3, 0.12, 7.0, 0.5);   planets[5] = &z;
	Planet x("saturn", 0.9, 0.8, 0.3, 0.1, 10.0, 0.15);   planets[6] = &x;
	Planet c("uranus", 0.4, 0.4, 1.0, 0.08, 13.0, 0.35);  planets[7] = &c;
	Planet v("neptune", 0.0, 0.0, 0.5, 0.05, 17.0, 0.35); planets[8] = &v;

	glutInit(&argc, argv);
	initopenGL();
	glutInitWindowSize(1100,600);
	glutInitWindowPosition(100,20);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Solar System");

	glutReshapeFunc(resize);

	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();

	return EXIT_SUCCESS;
}
