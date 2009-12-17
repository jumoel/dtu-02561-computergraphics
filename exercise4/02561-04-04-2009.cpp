#include <cassert>
#include <cmath>
#include <vector>

#include "GL/glut.h"

//size of selection buffer
#define BUFSIZE 512

enum component_type
{
	capacitor,
	resistor,
	transistor
};

struct component_t
{
	component_t(int t=0, int x=0, int y=0) : type(t), tx(x), ty(y), rx(0), sx(1), sy(1) {}

	int type;		//type of component
	int tx, ty;		//translation
	int rx;			//rotation in degrees
	float sx, sy;	//scale
};

static int width, height;
static bool ctrl_down = false;
static bool alt_down = false;
static bool shift_down = false;

static int mx = 0; //mouse position x
static int my = 0; //mouse position y

//list with current components
static std::vector<component_t> components;

//index of select components in above list, -1 means nothing is selected
static int selected = -1;

void draw_capacitor(void)
{
	//TODO: draw capacitor symbol
}

void draw_resistor(void)
{
	//TODO: draw resistor symbol
}

void draw_transistor(void)
{
	//TODO: draw transistor symbol
}

void draw_components(GLenum mode)
{
	//loop over components and draw them. Mode is either GL_RENDER or GL_SELECT
	for (size_t i=0; i<components.size(); ++i)
	{
		const component_t& c = components[i];

		//draw selected component in different color
		if (selected == i)
			glColor3f(1,0,0);
		else
			glColor3f(0,0,0);

		//TODO: draw components using their individual translation, rotation,
		//      and scale.
	}
}

int get_id(int hits, GLuint buffer[])
{
	if (hits == 0)
		return -1;

	//TODO: grab name from the select buffer and return it

	return 42;
}

void motion(int x, int y)
{
	x = x/2 - width/4;
	y = (height-y-1)/2 - height/4;

	//TODO: This function is called when the mouse is moved.
	//      Handle translation, rotation and scaling of the
	//      selected component here.

	//save mouse position for later
	mx = x; my = y;	

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	ctrl_down = (glutGetModifiers() & GLUT_ACTIVE_CTRL) != 0;
	alt_down = (glutGetModifiers() & GLUT_ACTIVE_ALT) != 0;
	shift_down = (glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0;

	if (selected != -1)
	{
		if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
			selected = -1;
		glutPostRedisplay();
		return;
	}

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	//TODO: Draw components with the 'select' render mode.
	//      Use gluPickMatrix to restrict drawing to a 16x16 pixels area near
	//      cursor (x,y).
	
	glutPostRedisplay();
} 

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	draw_components(GL_RENDER);   
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glClearColor(1.f,1.f,1.f,0.f);
	glColor3f(0.f, 0.f, 0.f);
	glLineWidth(2.f);

	width = w; height = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-w/4, w/4, -h/4, h/4);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case 27:
		  exit(0);
		  break;
	}

	glutPostRedisplay();
}

void menu(int c)
{
	components.push_back(component_t(c,mx,my));
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	//add some default components
	components.push_back(component_t(capacitor, 40, 0));
	components.push_back(component_t(resistor, 0, 0));
	components.push_back(component_t(transistor, -40, 0));

	//setup glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glutCreateMenu(menu);
	glutAddMenuEntry("New capacitor..",0); 
	glutAddMenuEntry("New resistor..",1); 
	glutAddMenuEntry("New transistor",2); 
	glutAttachMenu(GLUT_RIGHT_BUTTON); 
	glutReshapeFunc(reshape);
	glutDisplayFunc(display); 
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0; 
}