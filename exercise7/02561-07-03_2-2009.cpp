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

 enum {
  X, Y, Z, W
};
 enum {
  A, B, C, D
};


   GLfloat theta=0.0;

/* Find the plane equation given 3 points. */
void
findPlane(GLfloat plane[4],
  GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
  GLfloat vec0[3], vec1[3];

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

void display(void)

{
/* set clear color to white and clear window */

	//part 1 a & b
	GLfloat light[3]={10.0, 2.0, 0.0};

	GLfloat plane[4];
	GLfloat v1[3] = {-4.0, 2.0, 0.0};
	GLfloat v2[3] = {-4.0, 0.0, 5.0};
	GLfloat v3[3] = {-4.0, 1.0, 3.0};

	findPlane(plane, v1,v2,v3);
	
	GLfloat direction[3] = {-1.0, 0.0, 0.0};

	GLfloat m[16];

	double sin(), cos();

	int i;
	for(i=0;i<16;i++) m[i]=0.0;

	GLfloat a,b,c,d,dx,dy,dz;
	a=plane[0];
	b=plane[1];
	c=plane[2];
	d=plane[3];
	dx=direction[0];
	dy=direction[1];
	dz=direction[2];

	m[0]=b*dy+c*dz;
	m[1]=-a*dy;
	m[2]=-a*dz;
	m[4]=-b*dx;
	m[5]=a*dx+c*dz;
	m[6]=-b*dz;
	m[8]=-c*dx;
	m[9]=-c*dy;
	m[10]=a*dx+b*dy;
	m[12]=-d*dx;
	m[13]=-d*dy;
	m[14]=-d*dz;
	m[15]=a*dx+b*dy+c*dz;



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
		glVertex3f( 0.0, 0.0, 0.0);		
		glVertex3f( 3.0, 0.0, 0.0);
		glVertex3f( 3.0, 0.0, 3.0);	
		glVertex3f( 0.0, 0.0, 3.0);

		glVertex3f( 0.0, 0.0, 0.0);		
		glVertex3f( 3.0, 0.0, 0.0);
		glVertex3f( 3.0, 3.0, 0.0);	
		glVertex3f( 0.0, 3.0, 0.0);

		glVertex3f( 0.0, 0.0, 0.0);		
		glVertex3f( 0.0, 3.0, 0.0);
		glVertex3f( 0.0, 3.0, 3.0);	
		glVertex3f( 0.0, 0.0, 3.0);


		glVertex3f( 0.0, 0.0, 3.0);		
		glVertex3f( 3.0, 0.0, 3.0);
		glVertex3f( 3.0, 3.0, 3.0);	
		glVertex3f( 0.0, 3.0, 3.0);

		glVertex3f( 0.0, 3.0, 3.0);		
		glVertex3f( 0.0, 3.0, 0.0);
		glVertex3f( 3.0, 3.0, 0.0);	
		glVertex3f( 3.0, 3.0, 3.0);
		
		glVertex3f( 3.0, 3.0, 3.0);		
		glVertex3f( 3.0, 3.0, 0.0);
		glVertex3f( 3.0, 0.0, 0.0);	
		glVertex3f( 3.0, 0.0, 3.0);

		glEnd();

	glPushMatrix();

	glMultMatrixf(m);

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_QUADS);
		glVertex3f( 0.0, 0.0, 0.0);		
		glVertex3f( 3.0, 0.0, 0.0);
		glVertex3f( 3.0, 0.0, 3.0);	
		glVertex3f( 0.0, 0.0, 3.0);

		glVertex3f( 0.0, 0.0, 0.0);		
		glVertex3f( 3.0, 0.0, 0.0);
		glVertex3f( 3.0, 3.0, 0.0);	
		glVertex3f( 0.0, 3.0, 0.0);

		glVertex3f( 0.0, 0.0, 0.0);		
		glVertex3f( 0.0, 3.0, 0.0);
		glVertex3f( 0.0, 3.0, 3.0);	
		glVertex3f( 0.0, 0.0, 3.0);


		glVertex3f( 0.0, 0.0, 3.0);		
		glVertex3f( 3.0, 0.0, 3.0);
		glVertex3f( 3.0, 3.0, 3.0);	
		glVertex3f( 0.0, 3.0, 3.0);

		glVertex3f( 0.0, 3.0, 3.0);		
		glVertex3f( 0.0, 3.0, 0.0);
		glVertex3f( 3.0, 3.0, 0.0);	
		glVertex3f( 3.0, 3.0, 3.0);
		
		glVertex3f( 3.0, 3.0, 3.0);		
		glVertex3f( 3.0, 3.0, 0.0);
		glVertex3f( 3.0, 0.0, 0.0);	
		glVertex3f( 3.0, 0.0, 3.0);


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
