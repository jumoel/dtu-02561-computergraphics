/* Appendix 7  - 02561-01-07-2009-a7.cpp
*
*
*/

#include <GL/glut.h>

void initgl(void);

void display (void);
void axis (void);
void reshape (int w, int h);

float atx = -20.0;
float aty = 5.0;
float atz = 5.0;
float eyex = 20.0;
float eyey = 5.0;
float eyez = 5.0;
float upx = 0.0;
float upy = 1.0;
float upz = 0.0;

int main (int argc, char** argv) {
        glutInit (&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
        glutInitWindowSize (500, 500);
        glutInitWindowPosition (100, 100);
        glutCreateWindow ("Exercise 02561-01 Part 8");
        initgl ();
        glutDisplayFunc (display);
        glutReshapeFunc (reshape);
        glutMainLoop ();
        return 0;
}


void initgl (void) {
        glClearColor (0., 0., 0., 0.);
        glShadeModel (GL_FLAT);
        glMatrixMode (GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glLoadIdentity ();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
		gluPerspective(45.0, (GLsizei) w / (GLsizei) h, 10.0, 10000000.0);
    glMatrixMode (GL_MODELVIEW);
}


void display (void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.,1.,1.);
		
		gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz);
	  glPushMatrix();
		
    glBegin(GL_POINTS);
      glVertex3f(atx, aty, atz);
    glEnd();


	//glTranslatef(-5., -5., 5.);
   glColor3f (0.,0.,1.);
   glutWireCube (10.);
   glColor3f (1.,1.,1.); 
   glutSolidCube (10.);

    glPopMatrix();
    axis();
		
    glFlush ();
}


void axis (void) {
        /*Plot part of axis and an auxiliary line*/
        GLfloat v0[] = {0., 0., 0.};
        GLfloat vx[]= {10., 0., 0.};
        GLfloat vy[]= {0, 10., 0.};
        GLfloat vz[]= {0., 0., 10.};

        GLfloat v0x1[] = {1., 0., 0.};
        GLfloat vyx1[] = {1., 3., 0.};
        
        glPushAttrib(GL_CURRENT_BIT);
        glColor3f (0.8, 0.4, 0.4);
        glBegin (GL_LINES);
                glVertex3fv (v0);
                glVertex3fv (vx);
        glEnd ();
        
		glColor3f (0.4, 0.8, 0.4);
        glBegin (GL_LINES);
                glVertex3fv (v0);
                glVertex3fv (vy);
        glEnd ();
        
		glColor3f (0.4, 0.4, 0.8);
        glBegin (GL_LINES);
                glVertex3fv (v0);
                glVertex3fv (vz);
        glEnd ();
        
		glColor3f (0.2, 0.6, 0.2);
        glBegin (GL_LINES);
                glVertex3fv (v0x1);
                glVertex3fv (vyx1);
        glEnd ();
        glPopAttrib();
}