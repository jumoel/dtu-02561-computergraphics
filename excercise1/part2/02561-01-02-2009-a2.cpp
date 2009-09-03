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
	glColor3f (1., 1., 0.);
	glShadeModel (GL_SMOOTH);
}


void Display (void){
	
	float V[][2] ={
 		-5.,-5.,
		-5., 5.,
		 8., 5.,
		 8.,-5.
	};

	glClear (GL_COLOR_BUFFER_BIT);
	glBegin (GL_POLYGON);
		glVertex2fv (V[0]);
		glVertex2fv (V[1]);
		glVertex2fv (V[2]);
		glVertex2fv (V[3]);
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