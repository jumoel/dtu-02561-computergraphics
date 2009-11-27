#include <GL/glut.h>
#include <math.h>

#define M_PI 3.14159265358979323846

const float DEGTORAD = M_PI/180;

int width = 500;
int height = 500;

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

		if (i <= 90) {
			glColor3f(0.0, 1.0, 0.0);
		} else if (i <= 180) {
			glColor3f(0.0, 0.0, 1.0);
		} else if (i <= 270) {
			glColor3f(1.0, 0.0, 0.0);
		} else {
			glColor3f(1.0, 1.0, 1.0);
		}

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
  
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0,1.0,0.0);   
    
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();  
  glTranslatef(width/2,height/2,0);
  
  solid_circle(150.0);

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
  
  glutCreateWindow("Drawing a circle");  

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);

 // glutShowWindow();   
  glutMainLoop();

  return 0;
}