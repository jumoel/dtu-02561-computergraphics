/*Appendix A2  -  02561-01-02-2009
*
* The program draws a rectangle and a triangle
* 	
*
*/

#include <GL/glut.h>

void Init (void);
void Display (void);
void Reshape (int w, int h);

int main (int argc, char **argv){
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (600, 500);
	glutInitWindowPosition (10, 10);
	glutCreateWindow ("Hello World 2");
	
	glutDisplayFunc (Display);

	glutReshapeFunc (Reshape);
	glutMainLoop ();
	return 0;
}


void Init (void) {

	glClearColor (0., 0., 0., 0.);
	glShadeModel (GL_SMOOTH);
}


void Display (void){
	glClear (GL_COLOR_BUFFER_BIT);

	float V[][2] ={
 		-5.,-5.,
		-5., 5.,
		 8., 5.,
		 8.,-5.
	};

	glLoadIdentity();
	glTranslatef(-1.5, 0., 0.); 
	glRotatef(45., 0., 0., 1.);
	glTranslatef(1.5, 0., 0.);

	glColor3f (1., 1., 0.);
	glBegin (GL_POLYGON);
		glVertex2fv (V[0]);
		glVertex2fv (V[1]);
		glVertex2fv (V[2]);
		glVertex2fv (V[3]);
	glEnd ();

	float triangle[][2] ={
 		2.,  2.,
		5.,  2.,
		3.5, 5.
	};

	// Move the triangle
	glLoadIdentity();
	glTranslatef(6., 7., 0.);

	glBegin (GL_POLYGON);
		glColor3f (1., 0., 0.); // red
		glVertex2fv (triangle[0]);
		glColor3f (0., 1., 0.); // green
		glVertex2fv (triangle[1]);
		glColor3f (0., 0., 1.); // blue
		glVertex2fv (triangle[2]);
	glEnd ();

	glFlush ();
}

void Reshape (int w, int h){

	glViewport (0., 0., w, h);
	glClearColor (0., 0., 0., 0.);
	glColor3f (1., 1., 0.);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (-15., 15., -10., 15.);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}