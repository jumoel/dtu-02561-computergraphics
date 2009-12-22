/*02561-05-01  njc.01.10.2009*/

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

enum { X, Y, Z, W };
enum { A, B, C, D };

/* Find the plane equation given 3 points. Float edition */
void findPlane(double plane[4], double v0[3], double v1[3], double v2[3])
{
  float vec0[3], vec1[3];

  /* Need 2 vectors to find cross product. */
  vec0[X] = v1[X] - v0[X];
  vec0[Y] = v1[Y] - v0[Y];
  vec0[Z] = v1[Z] - v0[Z];

  vec1[X] = v2[X] - v0[X];
  vec1[Y] = v2[Y] - v0[Y];
  vec1[Z] = v2[Z] - v0[Z];

  /* find cross product to get A, B, and C of plane equation */
  plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
  plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
  plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

  plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}

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
  glEnable(GL_DEPTH_TEST);
}

void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Part 2 begin

  /* Find the equation for the plane defined by
   * the three points. */
  double eqn[4];
  double v0[3] = { 0.0, 0.0, 0.0 };
  double v1[3] = { 15.0, 0.0, 0.0 };
  double v2[3] = { 0.0, 15.0, 0.0 };
  findPlane(eqn, v0, v1, v2);

  glClipPlane (GL_CLIP_PLANE0, eqn);
  glEnable (GL_CLIP_PLANE0);

  // Part 2 end

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
	gluPerspective(90., (GLfloat) w / (GLfloat) h, 8., 50.); // Part 1: Change 4 -> 8
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (20., 10., 0., 0., 10., 0., 0., 0., 1.);
}
