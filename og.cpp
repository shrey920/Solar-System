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
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

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

// Function will compute new planet position
void Planet::glStuff(double t, double closeness) {
	this->x = sin(t*f)*a*1.5;
	this->y = cos(t*f)*a;

	glColor3d(c1,c2,c3);
	glPushMatrix();
	glTranslated(this->x,this->y,closeness);
	glRotated(50.0*t,0,0,1);
	glutSolidSphere(s1,20,20);

	glPopMatrix();
}

static double closeness = -40.0;
Planet** planets;
static double t; // time

static void display(void) // void
{
    //t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 9; i++) {
		(*planets[i]).glStuff(t, closeness);
	}

	// moon orbit calculation
	moon_x = -sin(t)*0.5+ planets[3]->x;
	moon_y = cos(t)*0.5+ planets[3]->y;

    float e=sqrt(planets[3]->x*planets[3]->x + planets[3]->y*planets[3]->y);
    float m=sqrt(moon_x*moon_x + moon_y*moon_y);
    float d= sqrt((moon_x-planets[3]->x)*(moon_x-planets[3]->x) +(moon_y-planets[3]->y)*(moon_y-planets[3]->y));


    if(closeness>-15.0 && m-e+d < 0.00001 ){
        setFont(GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0,1.0,1.0);
        unsigned char string[]="SOLAR ECLIPSE.";
        drawstring(-1.2,3.0,-10.0,string);
        for(int i=0;i<10000;i++) for(int j=0;j<5000;j++){}
    }

    if(closeness>-15.0 && e-m+d < 0.00001 ){
        setFont(GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0,1.0,1.0);
        unsigned char string[]="LUNAR ECLIPSE.";
        drawstring(-1.2,3.0,-10.0,string);
        for(int i=0;i<10000;i++) for(int j=0;j<5000;j++){}
    }



    // "Moon"
	glColor3d(0.7,0.7,0.7);
	glPushMatrix();
	glTranslatef(moon_x , moon_y , closeness);
	glRotatef(60,1,0,0);
	glRotatef(50.0*t,0,0,1);
	glutSolidSphere(0.1,20,20);
	glPopMatrix();
    t=t+0.001;
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
	Planet q("sun", 1.0, 0.6, 0.0, 0.0, 0.0, 1.5);        planets[0] = &q;
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

	glClearColor(0,0,0,0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat high_shininess[] = { 50.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);



	glutMainLoop();

	return EXIT_SUCCESS;
}
