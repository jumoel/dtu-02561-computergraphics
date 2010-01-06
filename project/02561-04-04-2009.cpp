#include <cassert>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#include "GL/glut.h"

#include "components.h"
#include "fileutil.h"
#include "component_types.h"
#include "settings.h"

//size of selection buffer
#define BUFSIZE 512



//list with current components
static std::vector<component_t> components;

//index of select components in above list, -1 means nothing is selected
static int selected = -1;

static bool ctrl_down = false;
static bool alt_down = false;
static bool shift_down = false;

static char *manualsave = "circuit.ccd";
static char *autosave = "autosave.ccd";

static int mx = 0; //mouse position x
static int my = 0; //mouse position y

static int mcx = 0; //mouse click position x
static int mcy = 0; //mouse click position y

static program_settings_t settings;

static const double zoomfactor = 0.02;

int transform_x(int x) {
  return (x - settings.width/2) / settings.zoom;
}

int transform_y(int y) {
  return (settings.height/2 - y - 1) / settings.zoom;
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

    if (mode == GL_SELECT) {
      glLoadName(i);
    }

    glPushMatrix();
    glTranslatef(c.tx, c.ty, 0.0);
    glRotatef(c.rx, 0.0, 0.0, 1.0);
    glScalef(c.sx, c.sy, 1.0);

		switch (c.type) {
			case capacitor:
				draw_capacitor(c);
				break;
			case resistor:
				draw_resistor(c);
				break;
			case transistor:
				draw_transistor(c);
				break;
      case wire:
        //draw_wire(c);
        break;
		}

        glPopMatrix();
	}
}

int get_id(int hits, GLuint buffer[])
{
	if (hits == 0)
		return -1;

	//TODO: grab name from the select buffer and return it
    GLuint *ptr = (GLuint *) buffer;
    GLint names;

    for (int i = 0; i < hits; i++)
    {
        names = *ptr;
        ptr = ptr + 3;
 
        for (int j = 0; j < names; j++)
        {
          //printf("Found: %d\n", ptr[j]);
        }

        return *(ptr++);
    }

	return -1;
}

void motion(int x, int y)
{
  x = transform_x(x);
  y = transform_y(y);

	// This function is called when the mouse is moved.
	// Handle translation, rotation and scaling of the
	// selected component here.

    // Scale
    if (selected != -1) {
        if (ctrl_down) {
            components[selected].sx = mcx;
            components[selected].sy = mcy;
        }
        else if (shift_down) {
            int dx = x - mcx;

            components[selected].rx += dx / 4;
        }
        else {
          components[selected].tx = x + settings.x_displ;
          components[selected].ty = y + settings.y_displ;
        }
    }

	//save mouse position for later
	mx = x; my = y;

	glutPostRedisplay();
}

void passivemotion(int x, int y)
{
	mx = transform_x(x);;
  my = transform_y(y);;
}

void mouse(int button, int state, int x, int y)
{

	ctrl_down = (glutGetModifiers() & GLUT_ACTIVE_CTRL) != 0;
	alt_down = (glutGetModifiers() & GLUT_ACTIVE_ALT) != 0;
	shift_down = (glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0;

  if(button == GLUT_LEFT_BUTTON) {
    mcx = transform_x(x);
    mcy = transform_y(y);
    printf("x: %d - y: %d\n", mcx, mcy);
  }

  if (selected != -1) {
	if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
		selected = -1;
  	glutPostRedisplay();
		return;
	}

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	GLint viewport[4];
    GLuint selectBuf[BUFSIZE];
    GLint hits;

	glGetIntegerv(GL_VIEWPORT, viewport);

	//TODO: Draw components with the 'select' render mode.
	//      Use gluPickMatrix to restrict drawing to a 16x16 pixels area near
	//      cursor (x,y).

  glSelectBuffer (BUFSIZE, selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames();
  glPushName(0);

  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();

  gluPickMatrix((GLdouble) x,
                (GLdouble) (viewport[3] - y),
                16.0,
                16.0, viewport);

  gluOrtho2D(-(settings.width / settings.zoom) / 2 + settings.x_displ,
              (settings.width / settings.zoom) / 2 + settings.x_displ,
             -(settings.height / settings.zoom) / 2 + settings.y_displ,
              (settings.height / settings.zoom) / 2 + settings.y_displ);

  draw_components(GL_SELECT);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glFlush();

  hits = glRenderMode(GL_RENDER);
  selected = get_id(hits, selectBuf);

  // Deletes the selected component if [ALT] is held down
  if (alt_down) { components.erase(components.begin() + selected); selected = -1; }

	glutPostRedisplay();
} 

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	draw_components(GL_RENDER);   
	glutSwapBuffers();
}

void update_ortho() {
  glutReshapeWindow(settings.width, settings.height);

  glViewport(0, 0, settings.width, settings.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  gluOrtho2D(-(settings.width / settings.zoom) / 2 + settings.x_displ,
              (settings.width / settings.zoom) / 2 + settings.x_displ,
             -(settings.height / settings.zoom) / 2 + settings.y_displ,
              (settings.height / settings.zoom) / 2 + settings.y_displ);
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  
}

void reshape(int w, int h)
{
	glClearColor(1.f,1.f,1.f,0.f);
	glColor3f(0.f, 0.f, 0.f);
	glLineWidth(2.f);

	settings.width = w; settings.height = h;

	update_ortho();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 19: // CTRL-S or CTRL-s
        save_file(manualsave, &components, &settings);
        printf("File saved as %s\n", manualsave);
      break;

    case '+':
      settings.zoom += zoomfactor * settings.zoom;
      update_ortho();
      break;
    case '-':
      settings.zoom -= zoomfactor * settings.zoom;
      update_ortho();

      break;
   
    case 27:
    case 'q':
    case 'Q':
	    exit(0);
	    break;
  }

	glutPostRedisplay();
}

void special_keyboard(int key, int x, int y)
{
  int mod = glutGetModifiers();
  switch (key) {
    case GLUT_KEY_F4:
      if (mod == GLUT_ACTIVE_ALT)
        exit(0);
      break;

    case GLUT_KEY_UP:
      settings.y_displ -= 1;
      update_ortho();
      break;
    case GLUT_KEY_DOWN:
      settings.y_displ += 1;
      update_ortho();
      break;
    case GLUT_KEY_LEFT:
      settings.x_displ += 1;
      update_ortho();
      break;
    case GLUT_KEY_RIGHT:
      settings.x_displ -= 1;
      update_ortho();
      break;

    
  }

	glutPostRedisplay();
}

void add_component(int c)
{
	components.push_back(component_t(c,mx,my));
	glutPostRedisplay();
}

void main_menu(int c) {
  switch (c) {
    case file_manualsave:
      comp_parse_file(manualsave, &components);
      settings_parse_file(manualsave, &settings);
      update_ortho();
      break;

    case file_autosave:
      comp_parse_file(autosave, &components);
      settings_parse_file(autosave, &settings);
      update_ortho();
      break;
  }

  glutPostRedisplay();
}

void create_menus() {
  int compmenu = glutCreateMenu(add_component);
	glutAddMenuEntry("Capacitor", capacitor); 
  glutAddMenuEntry("Resistor", resistor); 
	glutAddMenuEntry("Transistor", transistor);

  int menu = glutCreateMenu(main_menu);
  glutAddSubMenu("Insert ...", compmenu);
  glutAddMenuEntry("Load standard save file", file_manualsave);
  glutAddMenuEntry("Load automatic save file", file_autosave);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void autosave_file() {
  save_file(autosave, &components, &settings);
}

int main(int argc, char** argv)
{
  comp_parse_file(autosave, &components);
  settings_parse_file(autosave, &settings);

  atexit(autosave_file);

	//setup glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(settings.width, settings.height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	
  create_menus();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display); 
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
  glutPassiveMotionFunc(passivemotion);
	glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
	glutMainLoop();

	return 0; 
}