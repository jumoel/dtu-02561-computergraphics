/*02561-05-01  njc.01.10.2009*/

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "../../utility/mathfuncs.h"

void init (void);
void display (void);
void reshape (int w, int h);

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500,500);
	glutInitWindowPosition (100,100);

	glutCreateWindow ("02561-05-01");

	init (); 
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMainLoop ();

}

void init (void)
{
	glClearColor (1.,1.,1.,1.);
	glShadeModel (GL_FLAT);
	//glDisable (GL_DEPTH_TEST);
  glEnable(GL_DEPTH_TEST);
}

void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f (1., 0., 0.);

	glBegin (GL_LINES);
	  glVertex3f (0., 0., 0.);
	  glVertex3f  (15., 0., 0.);
	glEnd ();

	glBegin (GL_LINES);
	  glVertex3f (0., 0., 0.);
	  glVertex3f  (0., 15., 0.);
	glEnd ();

	glColor3f (0., 1., 0.);

	glBegin (GL_POLYGON);
	  glVertex3f (13., 13, 13);
	  glVertex3f (13., -13, 13);
	  glVertex3f (13., -13, -13);
	  glVertex3f (13., 13, -13);
	glEnd ();

	glColor3f (0., 1., 1.);
	glutSolidCube (6);

	glFlush ();
}


void reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90., (GLfloat) w / (GLfloat) h, 8., 50.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (20., 10., 0., 0., 10., 0., 0., 0., 1.);
}
