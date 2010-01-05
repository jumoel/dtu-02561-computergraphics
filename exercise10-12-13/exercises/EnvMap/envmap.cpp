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
GLuint tex;
GLuint texMapLocation[2];
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
  
  //draw skybox
  glUseProgram(background);
  texMapLocation[0] = glGetUniformLocation(background, "texMap");
  glUniform1i(texMapLocation[0],0);

  glutSolidCube(20.);

  //draw object
  glUseProgram(glass);
  texMapLocation[1] = glGetUniformLocation(glass, "texMap");
  glUniform1i(texMapLocation[1],0);
  // Send the cameraposition to the shader
  camerapos = glGetUniformLocation(glass, "camera");
  glUniform3f(camerapos, eyef[0], eyef[1], eyef[2]);

  refraction = glGetUniformLocation(glass, "refraction");
  glUniform1f(refraction, 0.5);

  //glutSolidTeapot(1.f);
  glutSolidSphere(1.0, 32, 32);
  //glutSolidTorus(0.5, 1., 32, 32);

  glUseProgram(0);
  glutSwapBuffers();

  check_gl_error();
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

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

  for (int i = 0; i < 6; i++) {
    int wi, he;

    void *data = load_ppm(cube_fn[i], wi, he);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i, 0, GL_RGB, wi, he, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data);
  }

  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  //create normalmap
  const char* normalmap_fn = "textures/normalmap.ppm";

  //TODO

  //create shaders
  background = create_program("background.vert", "background.frag");
  
  glass = create_program("glass.vert", "glass.frag");

  check_gl_error();
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