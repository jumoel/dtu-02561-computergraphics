/*
*
* 02561-10: Environment Mapping and Bump Mapping 
*
*/

#include "glsl_util.hpp"

#include <GL/glut.h>

#include "Graphics/TrackBall.h" 

#define TEX_SIZE 128

using namespace CGLA;
using namespace GFX;

static TrackBall* ball = NULL;
static int width, height;

static GLuint cubemap;
static GLuint background;
static GLuint glass;
static GLuint normalmap;
GLuint texMapLocation[3];
GLuint refraction;
GLuint camerapos;

void display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

  // Set up modelview transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  ball->do_spin();
  ball->set_gl_modelview();

  //get camera position
  Vec3f eye, at, up;
  ball->get_view_param(eye, at, up);
  GLfloat eyef[3] = { (GLfloat)eye[0], (GLfloat)eye[1], (GLfloat)eye[2] };

  CHECKGL(glActiveTexture(GL_TEXTURE0));
  CHECKGL(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap));

  CHECKGL(glActiveTexture(GL_TEXTURE1));
  CHECKGL(glBindTexture(GL_TEXTURE_2D, normalmap));
  
  //draw skybox
  CHECKGL(glUseProgram(background));

  CHECKGL(texMapLocation[0] = glGetUniformLocation(background, "texMap"));
  CHECKGL(glUniform1i(texMapLocation[0],0));

  glutSolidCube(20.);

  //draw object
  CHECKGL(glUseProgram(glass));

  CHECKGL(texMapLocation[1] = glGetUniformLocation(glass, "texMap"));
  CHECKGL(glUniform1i(texMapLocation[1],0));

  CHECKGL(texMapLocation[2] = glGetUniformLocation(glass, "normalMap"));
  CHECKGL(glUniform1i(texMapLocation[2],1));
 
  // Send the cameraposition to the shader
  CHECKGL(camerapos = glGetUniformLocation(glass, "camera"));
  CHECKGL(glUniform3f(camerapos, eyef[0], eyef[1], eyef[2]));

  /*
	Water: 1.33
	Glass: 2.04
	
	http://www.robinwood.com/Catalog/Technical/Gen3DTuts/Gen3DPages/RefractionIndexList.html
	*/
  CHECKGL(refraction = glGetUniformLocation(glass, "refraction"));
  CHECKGL(glUniform1f(refraction, 2.04));

  //glutSolidTeapot(1.f);
  glutSolidSphere(1.0, 32, 32);
  //glutSolidTorus(0.5, 1., 32, 32);

  CHECKGL(glUseProgram(0));
  glutSwapBuffers();
}


int old_state=GLUT_UP;
void mouse(int button, int state, int x, int y)
{
  if (old_state==GLUT_UP && state==GLUT_DOWN)
  {
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

void motion(int x, int y)
{
  if (old_state==GLUT_DOWN)
    ball->roll_ball(Vec2i(x,y));
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
  case '\033': exit(0); break;
  }
}

void animate(int x)
{
  glutPostRedisplay();
  glutTimerFunc(10, animate, 0);
}

void reshape(int w, int h)
{
  width = w;
  height = h;

  glViewport(0, 0, width, height);	

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.f, float(width)/height, 0.1, 50.);
  glMatrixMode(GL_MODELVIEW);

  delete ball;
  ball = new TrackBall(Vec3f(0), 5.f, 1.f, width, height);
}

void init(void)
{
  glClearColor(1,0,0,0);
  glEnable(GL_DEPTH_TEST);

  char *cube_fn[] = {
    "textures/cm_left.ppm", 
    "textures/cm_right.ppm", 
    "textures/cm_top.ppm", 
    "textures/cm_bottom.ppm", 
    "textures/cm_back.ppm", 
    "textures/cm_front.ppm" };

  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  glEnable(GL_TEXTURE_CUBE_MAP);

  CHECKGL(glGenTextures(1, &cubemap));
  CHECKGL(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap));

  for (int i = 0; i < 6; i++) {
    int wi, he;

    void *data = load_ppm(cube_fn[i], wi, he);

    CHECKGL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, wi, he, 0, GL_RGB, GL_UNSIGNED_BYTE, data));

    free(data);
  }

  //create normalmap
  const char* normalmap_fn = "textures/normalmap.ppm";

  CHECKGL(glGenTextures(1, &normalmap));
  CHECKGL(glBindTexture(GL_TEXTURE_2D, normalmap));

  int wi, he;
  void *data = load_ppm(normalmap_fn, wi, he);

  CHECKGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wi, he, 0, GL_RGB, GL_UNSIGNED_BYTE, data));

  free(data);
  
  //create shaders

  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  CHECKGL(background = create_program("background.vert", "background.frag"));
  CHECKGL(glass = create_program("glass.vert", "glass.frag"));
}

int main(int argc, char** argv)
{  
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(768, 768);
  glutInit(&argc, argv);
  glutCreateWindow("02561-10: Environment Mapping and Bump Mapping");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(10, animate, 0);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutReshapeFunc(reshape);

  glewInit();

  init();

  glutMainLoop();

  return 0;
}

//awk 10/11/08