#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>



GLfloat ctlpoints[7][3];
int showPoints = 0;
GLUnurbsObj *theNurb;

void init_surface(void)
{

	ctlpoints[0][0] = -10.0;
	ctlpoints[0][1] = 0.0;
	ctlpoints[0][2] = 0.0;

	ctlpoints[1][0] = -10.0;
	ctlpoints[1][1] = 5.0;
	ctlpoints[1][2] = 0.0;

	ctlpoints[2][0] = -5.0;
	ctlpoints[2][1] = 5.0;
	ctlpoints[2][2] = 0.0;

	ctlpoints[3][0] = 0.0;
	ctlpoints[3][1] = 10.0;
	ctlpoints[3][2] = 0.0;

	ctlpoints[4][0] = 5.0;
	ctlpoints[4][1] = 5.0;
	ctlpoints[4][2] = 0.0;

	ctlpoints[5][0] = 10.0;
	ctlpoints[5][1] = 5.0;
	ctlpoints[5][2] = 0.0;

	ctlpoints[6][0] = 10.0;
	ctlpoints[6][1] = 0.0;
	ctlpoints[6][2] = 0.0;


}
void nurbsError(GLenum errorCode)
{
    const GLubyte *estring;
    estring = gluErrorString(errorCode);
    fprintf (stderr, "Nurbs Error: %s\n", estring);
    exit (0);
}
void init(void)
{
    GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
   // glEnable(GL_DEPTH_TEST);
    //glEnable(GL_AUTO_NORMAL);
    //glEnable(GL_NORMALIZE);
    init_surface();
    theNurb = gluNewNurbsRenderer();
    //gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    //gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	gluNurbsProperty(theNurb, GLU_U_STEP, 100);
    gluNurbsCallback(theNurb, GLU_ERROR,(void) nurbsError);
}

//(GLvoid (*)()) nurbsError)
void display(void)
{
    GLfloat knots[11] = {0.0, 0.0, 0.0, 0.0,1.5,2.0,2.5, 3.0, 3.0, 3.0, 3.0};

    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glPushMatrix();
	//glRotatef(50.0, 1.,0.,0.);
	//glScalef (0.5, 0.5, 0.5);
	glColor3f(0.0,1.0,0.0);
	glLineWidth(2);
    gluBeginCurve(theNurb);
	gluNurbsCurve(theNurb,
        11, knots,
        3, &ctlpoints[0][0],
        4, GL_MAP1_VERTEX_3);


    gluEndCurve(theNurb);
    if (showPoints) {
        glPointSize(5.0);
        glDisable(GL_LIGHTING);
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_POINTS);
        for (i = 0; i < 7; i++) {
                glVertex3f(ctlpoints[i][0],
                    ctlpoints[i][1], ctlpoints[i][2]);
            
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();
    glFlush();
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	//gluLookAt(0.0,5.0,10.0,0.0,5.0,0.0,0.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(-15.0,15.0,-15.0,15.0);
	//gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glTranslatef (0.0, 0.0, -5.0);
}
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'c':
            case 'C':
            showPoints = !showPoints;
            glutPostRedisplay();
            break;
case 27:
    exit(0);
    break;
default:
    break;
    }
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc (keyboard);
    glutMainLoop();
    return 0;
}