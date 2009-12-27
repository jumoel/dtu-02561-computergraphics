#pragma warning (disable : 4786) 

/* Windows doesn't really support OpenGL beyond version 1.1 so we need
	 the library below to initialize extensions. */
#ifdef WIN32
#include <GL/extgl.h>
#else
#include <GL/gl.h>
#endif

#include <GL/glut.h>

#include "X3DSceneGraph/SceneGraphLoader.h"
#include "Graphics/PPMBitmap.h"
#include "Graphics/TrackBall.h"

#include "DisplayText.h"
#include "DisplayBuffer.h"

#include "CGLA/Mat4x4f.h"
#include "CGLA/Vec3f.h"


using namespace std;
using namespace CGLA;
using namespace GFX;

namespace 
{
	// Data types --------------------------------------------------

	// Struct containing simple viewer information
	struct Viewer 
	{
		Vec3f eye;
		Vec3f lookat;
		Vec3f up;

		Viewer(Vec3f _eye, Vec3f _lookat, Vec3f _up):
			eye(_eye), lookat(_lookat), up(_up) {}
	};


	// Global variables --------------------------------------------

	X3DSceneGraph::Scene *x3dobj; // The object that is reflected and 
	// which casts shadow.

	TrackBall *ball;              // Used to control viewing

	DisplayText displaytext;      // Render frame rates and text

	DisplayBuffer *displaybuffer; // Used to copy stencil, alpha, depth buffers 
	// to rgb buffer
	const int WINSIZE = 512;

	const int texture_size = 256; // Size of shadow texture

	GLubyte shadow_texture[texture_size*texture_size*4];  // Texture for shadow


	int display_mode = 0;         // 0 scene, 1 alpha, 2 stencil, 3 depth

	int shadow_type = 1;          // 0 Flattening, 1 projected

	int id;                       // id for text drawing.

	GLuint proj_tex;              // Projective texture for shadow.

	GLuint plane_tex;             // (marble) Texture for plane. 
}

void display_x3dobj(const Vec3f& position) {
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	x3dobj->draw();
	glPopMatrix();
}


void init_light_sources() {

	glEnable(GL_LIGHTING);
	GLfloat white_light[] = {1,1,1.0,1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE,white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR,white_light);
	glEnable(GL_LIGHT0);

	// Part 2: Here you need to set up second light
	// that illuminates reflection from below.

	GLfloat white_light2[] = {.6,.6,.6,.6};
	glLightfv(GL_LIGHT1, GL_DIFFUSE,white_light2);
	glLightfv(GL_LIGHT1, GL_SPECULAR,white_light2);

}


void bind_light_pos() {
	// Set light that illuminates teapot
	GLfloat light_position[] = {0,300,0,1.0};
	glLightfv(GL_LIGHT0, GL_POSITION,light_position);

	// Part 2: Here you need to bind position of second light

	GLfloat light_position2[] = {0,-300,0,1.0};
	glLightfv(GL_LIGHT1, GL_POSITION,light_position2);

}

 

void init_reflector_texture() {	

	glGenTextures(1, &plane_tex);
	glBindTexture(GL_TEXTURE_2D, plane_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
 									GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	static PPMBitmap picture("xamp23.ppm");
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 
										picture.get_width(),
										picture.get_height(), 
										GL_RGBA, GL_UNSIGNED_BYTE, 
										picture.get());
}

void draw_reflecting_plane(float col, bool do_texture=false) 
{
	
	int s = 200;  // size of mirror

	if(do_texture)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, plane_tex);
		}

	glBegin(GL_QUADS);		
	
	for (int j=0;j<4;j++)  
		{ 
			float x,y,z,u,v;		
			if (j==0||j==3) x = s; else x = -s;
			if (j==0||j==1) z = s; else z = -s;	
			y = 0;
			u = ((x+s)/(abs(s)*2));
			v = ((z+s)/(abs(s)*2));

			if(do_texture)
				glTexCoord2f(u,v);
			glColor4f( col,col,col,1);									
			glVertex3f(x, y, -z);			
		}  		
	glEnd();

	if(do_texture)
		glDisable(GL_TEXTURE_2D);
}


// --------------------------------------------------
// Part 1 - projected shadows.
// --------------------------------------------------


void set_from_light_perspective(const Viewer& v) 
{
	// In this function, you compute the extent of the object
	// and set up perspective appropriately.
}


void init_proj_shadow_texture() 
{
	// Create the texture used for the projective shadow.
}


void make_proj_shadow_texture(const Viewer& viewer) 
{
	// Render the object, and copy to the texture.
}

void draw_proj_shadow(const Viewer& viewer) 
{
	// First draw the plane textured with the teapot texture 
	// to the alpha buffer. Then draw the plane with marble texture
	// and blend using alpha values.
}



// --------------------------------------------------
// Part 2 - reflections and flattening shadows.
// --------------------------------------------------


void draw_mirror(const Vec3f& v) 
{
}

void draw_flatten_shadow(const Vec3f& v) 
{
}

// --------------------------------------------------
// GLUT callback functions 
// --------------------------------------------------

bool freeze = false;

void display() {

	static Viewer viewer(
		Vec3f(0,300,0), // Eye 
		Vec3f(0,0,0),   // Lookat 
		Vec3f(0,0,1)    // Up
	);

	// Generate position for x3d object
	static int counter=42;
	float x = sin(counter/180.0f*M_PI)*120 + cos(counter/70.0f*M_PI)*50;
	float y = cos(counter/180.0f*M_PI)* 40 + sin(counter/70.0f*M_PI)*40;
	float z = cos(counter/180.0f*M_PI)*120 + cos(counter/70.0f*M_PI)*50;
	viewer.lookat = Vec3f(x,y,z);
	if(!freeze) counter++;

	// Create shadow texture
	if (shadow_type==1)
		make_proj_shadow_texture(viewer);

	glClearColor(0,0,0,0);
	// Set viewport and clear all buffers.
	glViewport(0,0,WINSIZE,WINSIZE);	
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	

	// Set up modelview transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ball->do_spin();
	ball->set_gl_modelview();
	
	// Bind light pos - the light is relative to scene.
	bind_light_pos();

 	// Draw mirror
	draw_mirror(viewer.lookat);	

	// Draw shadow
	glDisable(GL_LIGHTING);
	switch (shadow_type) {
		case 0: break;
		case 1: draw_proj_shadow(viewer); break;
		case 2:	draw_flatten_shadow(viewer.lookat); break;
	}

	// Draw object 
	glEnable(GL_LIGHTING);
	x3dobj->enableMaterial(true);
	display_x3dobj(viewer.lookat);

	// Replace image by some other buffer.
	// Show stencil, depth or alpha buffer
	switch (display_mode) {
		case 0:
			displaytext.updateText(id,"Complete scene");
			break;
		case 1:
			displaybuffer->drawAlphaBuffer();
			displaytext.updateText(id,"Alpha buffer");
			break;
		case 2:
			displaybuffer->drawStencilBuffer();
			displaytext.updateText(id,"Stencil buffer");
			break;
		case 3:
			displaybuffer->drawDepthBuffer();
			displaytext.updateText(id,"Depth buffer");
			break;
	}
	// Show framerate
	displaytext.draw();

	glutSwapBuffers();
}


int old_state=GLUT_UP;
void mouse(int button, int state, int x, int y) {
	if (old_state==GLUT_UP && state==GLUT_DOWN) {
		if (button==GLUT_LEFT_BUTTON) 
			ball->grab_ball(ROTATE_ACTION,Vec2i(x,y));
		else if (button==GLUT_MIDDLE_BUTTON) 
			ball->grab_ball(ZOOM_ACTION,Vec2i(x,y));
		else if (button==GLUT_RIGHT_BUTTON) 
			ball->grab_ball(PAN_ACTION,Vec2i(x,y));
	}
	if (old_state==GLUT_DOWN && state==GLUT_UP)
		ball->release_ball();
	old_state=state;
}

void motion(int x, int y) {
	if (old_state==GLUT_DOWN)
		ball->roll_ball(Vec2i(x,y));
}

void keyboard(unsigned char key, int x, int y) {
  switch(key) {
	case '\033': exit(0); break;
	case ' ':display_mode = 0; break;
	case 'a':display_mode = 1; break;
	case 'f': freeze = !freeze; break;
	case 's':display_mode = 2; break;
	case 'd':display_mode = 3; break;
	case 'p':shadow_type=(shadow_type+1)%3; break;
  }
}


void animate(int x) {
  glutPostRedisplay();
	glutTimerFunc(10, animate, 0);
}


int main(int argc, char** argv)
{  
	// Create a DisplayBuffer object (used to display depth, stencil and
	// alpha buffers).
	displaybuffer = new DisplayBuffer(WINSIZE,WINSIZE);

	// Initialize GLUT
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|
											GLUT_DEPTH|GLUT_STENCIL);
	glutInitWindowSize(WINSIZE, WINSIZE);
	glutInit(&argc, argv);
	glutCreateWindow("Shadows & Reflections");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(10, animate, 0);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(0);

	// Load Model
	x3dobj = X3DSceneGraph::getModel("teapot.x3d");	
	x3dobj->init();
	cout << "x3dobj bounding volume" << x3dobj->bv.radius << endl << endl;

	// Interactive Display
	id = displaytext.addText("");
	displaytext.addFramerate();

	// Print OpenGL info
	int val;
	glGetIntegerv(GL_DEPTH_BITS, &val);
	cout << "Depth bits: " << val << endl;
	glGetIntegerv(GL_STENCIL_BITS, &val);
	cout << "Stencil bits: " << val << endl;
	glGetIntegerv(GL_ALPHA_BITS, &val);
	cout << "Alpha bits: " << val << endl;


	// Setup basic OpenGL state
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,1.5,2500.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);


	// Initialize Trackball
	ball = new TrackBall(Vec3f(0), WINSIZE, WINSIZE, 750, 750);


	init_proj_shadow_texture();
	init_reflector_texture();
	init_light_sources();

	glutMainLoop();

	return 0;
}
