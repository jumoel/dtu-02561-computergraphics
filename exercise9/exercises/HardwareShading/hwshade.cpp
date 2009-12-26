#pragma warning (disable : 4786) 

/* Windows doesn't really support OpenGL beyond version 1.1 so we need
	 the library below to initialize extensions. */
#ifdef WIN32
#include <GL/extgl.h>
#else
#include <GL/gl.h>
#endif

#include <GL/glut.h>
#include <cmath>

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

    Vec3f ray = v.lookat - v.eye;
    double dist = ray.length();
    double fov = 2 * asin(x3dobj->bv.radius / dist);

    gluPerspective(180*fov/M_PI,
                   1.0,
                   dist - x3dobj->bv.radius,
                   dist + x3dobj->bv.radius);
}


void init_proj_shadow_texture() 
{
	// Create the texture used for the projective shadow.

 	glGenTextures(1, &proj_tex);
	glBindTexture(GL_TEXTURE_2D, proj_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                                   GL_NEAREST);
}


void make_proj_shadow_texture(const Viewer& viewer) 
{
	// Render the object, and copy to the texture.

    glViewport(0,0,texture_size,texture_size);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    set_from_light_perspective(viewer);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Since the viewer is defined from the light source we are able to use
    // it directly in the gluLookAt function
    gluLookAt(viewer.eye[0],
              viewer.eye[1],
              viewer.eye[2],
              viewer.lookat[0],
              viewer.lookat[1],
              viewer.lookat[2],
              viewer.up[0],
              viewer.up[1],
              viewer.up[2]);

    // disable light to get a black and white image (shadows...)
    glDisable(GL_LIGHTING);

    // Render to texture
    glColor4f(.3,.3,.3,.3);
    x3dobj->enableMaterial(false);

	// Since we don't want shadows above the tea pot this clip plane is added
	GLdouble equation[4] = { 0, 1, 0, 0 };
	glClipPlane(GL_CLIP_PLANE0, equation);
	glEnable(GL_CLIP_PLANE0);
	
    display_x3dobj(viewer.lookat);

	glDisable(GL_CLIP_PLANE0);
    x3dobj->enableMaterial(true);

    glBindTexture(GL_TEXTURE_2D, proj_tex);
//   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, texture_size, texture_size, 0);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
}

void draw_proj_shadow(const Viewer& viewer) 
{
	// First draw the plane textured with the teapot texture 
	// to the alpha buffer. Then draw the plane with marble texture
	// and blend using alpha values.

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.5,0.5,0);
    glScalef(0.5,0.5,1);
    set_from_light_perspective(viewer);
    gluLookAt(viewer.eye[0],
              viewer.eye[1],
              viewer.eye[2],
              viewer.lookat[0],
              viewer.lookat[1],
              viewer.lookat[2],
              viewer.up[0],
              viewer.up[1],
              viewer.up[2]);

    glMatrixMode(GL_MODELVIEW);

    float gls[] = {1, 0, 0, 0};
    float glt[] = {0, 1, 0, 0};
    float glr[] = {0, 0, 1, 0};
    float glq[] = {0, 0, 0, 1};

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, proj_tex);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_PLANE, gls);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, glt);
    glTexGenfv(GL_R, GL_OBJECT_PLANE, glr);
    glTexGenfv(GL_Q, GL_OBJECT_PLANE, glq);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glDisable(GL_LIGHTING);

    // Draw Quad

    glDepthFunc(GL_LEQUAL);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);

    draw_reflecting_plane(0);
    glDisable(GL_TEXTURE_2D);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_LIGHTING);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);

 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ONE);
    draw_reflecting_plane(1, true);
    glDisable(GL_BLEND);
}


// --------------------------------------------------
// Part 2 - reflections and flattening shadows.
// --------------------------------------------------


void draw_mirror(const Vec3f& v) 
{
//  Stencil test

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

	draw_reflecting_plane(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
//  Our code ...

	glPushMatrix();
	glScalef(1.0, -1.0, 1.0);
	glFrontFace(GL_CW);

	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
//  clip plane - part 6

	GLdouble equation[4] = { 0, 1, 0, 0 };
	glClipPlane(GL_CLIP_PLANE0, equation);
	glEnable(GL_CLIP_PLANE0);
    
	display_x3dobj(v);

	glDisable(GL_CLIP_PLANE0);

	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);

	glPopMatrix();

//  Disabling stencil test

	glDisable(GL_STENCIL_TEST);
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
	// glDisable(GL_LIGHTING);
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
	cout << endl
         << "x3dobj bounding volume "
         << x3dobj->bv.radius
         << endl
         << endl;

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
