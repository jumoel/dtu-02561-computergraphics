
/*	02501-03-02.cpp (= OGLPG Example 8.1 material.c)
 * This program demonstrates the use of the GL lighting model.
 * Several objects are drawn using different material characteristics.
 * A single light source illuminates the objects.
 */

#include <stdlib.h>
#include <GL/glut.h>

/*  Initialize z-buffer, projection matrix, light source, 
 *  and lighting model.  Do not specify a material property here.
 */
void myinit(void)
{
    GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 0.0, 3.0, 2.0, 0.0 };
    GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat local_view[] = { 0.0 };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClearColor(0.0, 0.1, 0.1, 0.0);
}

/*  Draw twelve spheres in 3 rows with 4 columns.  
 *  The spheres in the first row have materials with no ambient reflection.
 *  The second row has materials with significant ambient reflection.
 *  The third row has materials with colored ambient reflection.
 *
 *  The first column has materials with blue, diffuse reflection only.
 *  The second column has blue diffuse reflection, as well as specular
 *  reflection with a low shininess exponent.
 *  The third column has blue diffuse reflection, as well as specular
 *  reflection with a high shininess exponent (a more concentrated highlight).
 *  The fourth column has materials which also include an emissive component.
 *
 *  glTranslatef() is used to move spheres to their appropriate locations.
 */

void display(void)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat no_shininess[] = { 0.0 };
    GLfloat low_shininess[] = { 5.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {0.9, 0.9, 0.9, 0.0};
    
	GLfloat std[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat stdS[] = { 5.0};
	GLfloat stdE[] = { 0.2, 0.2, 0.2, 0.0 };
	GLfloat some0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat some1[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat some2[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat some3[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat some4[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat S0[] = { 0.0};
	GLfloat S1[] = { 5.0};
	GLfloat S2[] = { 20.0};
	GLfloat S3[] = { 50.0};
	GLfloat S4[] = { 100.0};
	GLfloat E0[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat E1[] = { 0.2, 0.2, 0.2, 0.0 };
	GLfloat E2[] = { 0.5, 0.5, 0.5, 0.0 };
	GLfloat E3[] = { 0.7, 0.7, 0.7, 0.0 };
	GLfloat E4[] = { 1.0, 1.0, 1.0, 0.0 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*  draw sphere in first row, first column
 *  diffuse reflection only; no ambient or specular  
 */
    glPushMatrix();
    glTranslatef (-3.75, 3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, some0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in first row, second column
 *  diffuse and specular reflection; low shininess; no ambient
 */
    glPushMatrix();
    glTranslatef (-1.25, 3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, some1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in first row, third column
 *  diffuse and specular reflection; high shininess; no ambient
 */
    glPushMatrix();
    glTranslatef (1.25, 3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, some2);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in first row, fourth column
 *  diffuse reflection; emission; no ambient or specular reflection
 */
    glPushMatrix();
    glTranslatef (3.75, 3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, some3);
     glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

	/*  draw sphere in first row, 5th column
 *  diffuse reflection; emission; no ambient or specular reflection
 */
    glPushMatrix();
    glTranslatef (6.25, 3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, some4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in second row, first column
 *  ambient and diffuse reflection; no specular  
 */
    glPushMatrix();
    glTranslatef (-3.75, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, some0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in second row, second column
 *  ambient, diffuse and specular reflection; low shininess
 */
    glPushMatrix();
    glTranslatef (-1.25, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, some1);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in second row, third column
 *  ambient, diffuse and specular reflection; high shininess
 */
    glPushMatrix();
    glTranslatef (1.25, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, some2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in second row, fourth column
 *  ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (3.75, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, some3);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();


	/*  draw sphere in second row, 5th column
 *  ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (6.25, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, some4);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in third row, first column
 *  colored ambient and diffuse reflection; no specular  
 */
    glPushMatrix();
    glTranslatef (-3.75, -3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, some0);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in third row, second column
 *  colored ambient, diffuse and specular reflection; low shininess
 */
    glPushMatrix();
    glTranslatef (-1.25, -3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, some1);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in third row, third column
 *  colored ambient, diffuse and specular reflection; high shininess
 */
    glPushMatrix();
    glTranslatef (1.25, -3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, some2);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in third row, fourth column
 *  colored ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (3.75, -3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, some3);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in third row, 5th column
 *  colored ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (6.25, -3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, some4);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();


/*  draw sphere in 4th row, first column
 *  colored ambient and diffuse reflection; no specular  
 */
    glPushMatrix();
    glTranslatef (-3.75, -6.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, S0);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 4th row, second column
 *  colored ambient, diffuse and specular reflection; low shininess
 */
    glPushMatrix();
    glTranslatef (-1.25, -6.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, S1);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 4th row, third column
 *  colored ambient, diffuse and specular reflection; high shininess
 */
    glPushMatrix();
    glTranslatef (1.25, -6.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, S2);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 4th row, fourth column
 *  colored ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (3.75, -6.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, S3);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 4th row, 5th column
 *  colored ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (6.25, -6.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, S4);
    glMaterialfv(GL_FRONT, GL_EMISSION, stdE);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();



	/*  draw sphere in 5th row, first column
 *  colored ambient and diffuse reflection; no specular  
 */
    glPushMatrix();
    glTranslatef (-3.75, -9.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, E0);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 5th row, second column
 *  colored ambient, diffuse and specular reflection; low shininess
 */
    glPushMatrix();
    glTranslatef (-1.25, -9.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, E1);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 5th row, third column
 *  colored ambient, diffuse and specular reflection; high shininess
 */
    glPushMatrix();
    glTranslatef (1.25, -9.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, E2);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 5th row, fourth column
 *  colored ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (3.75, -9.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, E3);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

/*  draw sphere in 5th row, 5th column
 *  colored ambient and diffuse reflection; emission; no specular
 */
    glPushMatrix();
    glTranslatef (6.25, -9.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, std);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, std);
    glMaterialfv(GL_FRONT, GL_SPECULAR, std);
    glMaterialfv(GL_FRONT, GL_SHININESS, stdS);
    glMaterialfv(GL_FRONT, GL_EMISSION, E4);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();





    glFlush();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= (h * 2))
	glOrtho (-6.0, 9.0, -6.0*((GLfloat)h*2)/(GLfloat)w, 
	    3.0*((GLfloat)h*2)/(GLfloat)w, -10.0, 10.0);
    else
	glOrtho (-6.0*(GLfloat)w/((GLfloat)h*2), 
	    6.0*(GLfloat)w/((GLfloat)h*2), -3.0, 3.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutCreateWindow(argv[0]);
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}

