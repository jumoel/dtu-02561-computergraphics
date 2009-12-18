/*
 *  shadow.c
 *  This program draws a polygon and its shadow.
 */


#include <stdlib.h>           
#ifdef __APPLE__
#include <GLUT/glut.h>              /* glut.h */
#else
#include <GL/glut.h>
#endif

   GLfloat theta=0.0;
double offset = 4.0;
void display(void)

{
/* set clear color to white and clear window */

	//part 1 a & b
	GLfloat light[3]={10.0+offset, 2.0, 0.0};


	GLfloat m[16];

	double sin(), cos();

	int i;
	for(i=0;i<16;i++) m[i]=0.0;

	m[0]=m[5]=m[10]=1.0;
	m[3]=-1.0/light[0];
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

/* set drawing/fill  color to red */

	glColor3f(1.0, 0.0, 0.0);

/* set up standard orthogonal view with clipping */
/* box as cube of side 2 centered at origin */

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

/* define unit square polygon */

	glBegin(GL_QUADS);
		glVertex3f( 0.0+offset, 0.0, 0.0);		
		glVertex3f( 3.0+offset, 0.0, 0.0);
		glVertex3f( 3.0+offset, 0.0, 3.0);	
		glVertex3f( 0.0+offset, 0.0, 3.0);

		glVertex3f( 0.0+offset, 0.0, 0.0);		
		glVertex3f( 3.0+offset, 0.0, 0.0);
		glVertex3f( 3.0+offset, 3.0, 0.0);	
		glVertex3f( 0.0+offset, 3.0, 0.0);

		glVertex3f( 0.0+offset, 0.0, 0.0);		
		glVertex3f( 0.0+offset, 3.0, 0.0);
		glVertex3f( 0.0+offset, 3.0, 3.0);	
		glVertex3f( 0.0+offset, 0.0, 3.0);


		glVertex3f( 0.0+offset, 0.0, 3.0);		
		glVertex3f( 3.0+offset, 0.0, 3.0);
		glVertex3f( 3.0+offset, 3.0, 3.0);	
		glVertex3f( 0.0+offset, 3.0, 3.0);

		glVertex3f( 0.0+offset, 3.0, 3.0);		
		glVertex3f( 0.0+offset, 3.0, 0.0);
		glVertex3f( 3.0+offset, 3.0, 0.0);	
		glVertex3f( 3.0+offset, 3.0, 3.0);
		
		glVertex3f( 3.0+offset, 3.0, 3.0);		
		glVertex3f( 3.0+offset, 3.0, 0.0);
		glVertex3f( 3.0+offset, 0.0, 0.0);	
		glVertex3f( 3.0+offset, 0.0, 3.0);

		glEnd();

	glPushMatrix();
	glTranslatef(light[0], light[1],light[2]);
	glMultMatrixf(m);
	glTranslatef(-light[0], -light[1],-light[2]);
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_QUADS);
		glVertex3f( 0.0+offset, 0.0, 0.0);		
		glVertex3f( 3.0+offset, 0.0, 0.0);
		glVertex3f( 3.0+offset, 0.0, 3.0);	
		glVertex3f( 0.0+offset, 0.0, 3.0);

		glVertex3f( 0.0+offset, 0.0, 0.0);		
		glVertex3f( 3.0+offset, 0.0, 0.0);
		glVertex3f( 3.0+offset, 3.0, 0.0);	
		glVertex3f( 0.0+offset, 3.0, 0.0);

		glVertex3f( 0.0+offset, 0.0, 0.0);		
		glVertex3f( 0.0+offset, 3.0, 0.0);
		glVertex3f( 0.0+offset, 3.0, 3.0);	
		glVertex3f( 0.0+offset, 0.0, 3.0);


		glVertex3f( 0.0+offset, 0.0, 3.0);		
		glVertex3f( 3.0+offset, 0.0, 3.0);
		glVertex3f( 3.0+offset, 3.0, 3.0);	
		glVertex3f( 0.0+offset, 3.0, 3.0);

		glVertex3f( 0.0+offset, 3.0, 3.0);		
		glVertex3f( 0.0+offset, 3.0, 0.0);
		glVertex3f( 3.0+offset, 3.0, 0.0);	
		glVertex3f( 3.0+offset, 3.0, 3.0);
		
		glVertex3f( 3.0+offset, 3.0, 3.0);		
		glVertex3f( 3.0+offset, 3.0, 0.0);
		glVertex3f( 3.0+offset, 0.0, 0.0);	
		glVertex3f( 3.0+offset, 0.0, 3.0);


		glEnd();
	glPopMatrix();

/* Swap buffers */

	glutSwapBuffers();

}



int main(int argc, char** argv)
{

/* Initialize mode and open a window in upper left corner of screen */
/* Window title is name of program (arg[0]) */

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH); 
	glutInitWindowSize(500,500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("shadow");
	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

}
