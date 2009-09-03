#include <GL/glut.h>
#include <math.h>

#define M_PI 3.14159265358979323846

const float DEGTORAD = M_PI/180;

int width = 500;
int height = 800;

void circle(float radius) {
	glBegin(GL_LINE_LOOP);
	
	for(int i = 0; i < 360; i++) {
		float degree_in_rad = (float)i*DEGTORAD;

		glVertex2f(	cos(degree_in_rad)*radius, 
					sin(degree_in_rad)*radius);
	}

	glEnd();
}

void solid_circle(float radius) {
	float x_origin = 0.;
	float y_origin = 0.;

	float x_old = x_origin;
	float y_old = y_origin;

	glBegin(GL_TRIANGLES);

	for(int i = 0; i <= 360; i++) {
		float degree_in_rad = (float)i*DEGTORAD;

		float x = cos(degree_in_rad)*radius;		
		float y = sin(degree_in_rad)*radius;

		glVertex2f(x_origin, y_origin);
		glVertex2f(x, y);
		glVertex2f(x_old, y_old);

		x_old = x;
		y_old = y;
	}

	glEnd();
}

void solid_moon(float radius) {
	float x_origin = 0.;
	float y_origin = 0.;

	float x_old = x_origin;
	float y_old = y_origin;

	glBegin(GL_TRIANGLES);

	for(int i = 180; i <= 360; i++) {
		float degree_in_rad = (float)i*DEGTORAD;

		float x = cos(degree_in_rad)*radius;		
		float y = sin(degree_in_rad)*radius;

		glVertex2f(x_origin, y_origin);
		glVertex2f(x, y);
		glVertex2f(x_old, y_old);

		x_old = x;
		y_old = y;
	}

	glEnd();
}

void display() {
	// White background
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	
	glColor3f(0.19,0.38,0.24);
	
	glLoadIdentity();
	glTranslatef(300.0, 600.0, 0.0);
	solid_circle(120.0);

	glLoadIdentity();
	glTranslatef(120.0, 450.0, 0.0);
	solid_circle(60.0);

	glLoadIdentity();
	glTranslatef(300.0, 300.0, 0.0);
	solid_circle(30.0);

	// Eyes
	glColor3f(1.0, 1.0, 1.0);

	glLoadIdentity();
	glTranslatef(290.0, 305.0, 0.0);
	solid_circle(6.5);

	glLoadIdentity();
	glTranslatef(310.0, 305.0, 0.0);
	solid_circle(6.5);

	// Eyes
	glColor3f(0.0, 0.0, 0.0);

	glLoadIdentity();
	glTranslatef(289.0, 307.0, 0.0);
	solid_circle(3.5);

	glLoadIdentity();
	glTranslatef(309.0, 307.0, 0.0);
	solid_circle(3.5);

	// Mouth
	glColor3f(0.0, 0.0, 0.0);

	glLoadIdentity();
	glTranslatef(300.0, 295.0, 0.0);
	solid_moon(22.0);

	glColor3f(1.0, 1.0, 1.0);

	glTranslatef(0.0, -2.0, 0.0);
	solid_moon(18.0);

	// Hat
	glLoadIdentity();
	glTranslatef(270., 320., 0.);

	float triangle[][2] ={
 		0.,  0.,
		60.,  0.,
		30., 30.
	};

	glBegin (GL_POLYGON);
		glColor3f (1., 0., 0.); // red
		glVertex2fv (triangle[0]);
		glColor3f (0., 1., 0.); // green
		glVertex2fv (triangle[1]);
		glColor3f (0., 0., 1.); // blue
		glVertex2fv (triangle[2]);
	glEnd ();

	glFlush();
}


void reshape(int w, int h)  {         

  width = w;
  height = h;

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  glOrtho(0, width, 0, height, -1000, 1000 );

  glViewport(0, 0, width, height);  
}


int main(int argc, char **argv) {    

  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB);    
  
  glutCreateWindow("Bitdrengen");  

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);

 // glutShowWindow();   
  glutMainLoop();

  return 0;
}