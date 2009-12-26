#include <iostream>
#include <math.h>

#include <GL/glut.h>

#include "CGLA/Vec3f.h"
#include "Components/ResourceLoader.h" 

#include "Graphics/TrackBall.h"
#include "TextureVolume.h"


using namespace std;
using namespace CGLA;
using namespace GFX;
using namespace CMP;

namespace 
{
  const int WINX = 500;
  const int WINY = 500;
  
  TrackBall *ball;
  TextureVolume *tex_vol;
  
  int iso = 40;
  bool pan = false;
  Manip manipulation = PERLIN_ADD;
  int tool_size = 10;
  bool left_mouse_button_down;
  bool marching = false;
};

bool zpick(const Vec2i& p, Vec3f& wp)
{
  return true;
}

void sculpt(const Vec2i mp)
{
  Vec3f p;
  if(zpick(mp, p))
    tex_vol->manipulate(p,tool_size,manipulation);
}

void reset_proj(float dist) 
{  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45,1.0,10,dist);
  glMatrixMode(GL_MODELVIEW);
}

void idle()
{
  ball->do_spin();
  glutPostRedisplay();
}


void display()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glPushMatrix();
  ball->set_gl_modelview();
  if(marching)
    tex_vol->gl_draw_polygons();
  else
    {
      Vec3f eye,centre,up;
      glColor3f(1,1,1);
      ball->get_view_param(eye,centre,up);
      tex_vol->gl_draw_stack(eye-centre, iso);
    }
  glPopMatrix();
  
  glutSwapBuffers();
}


static void key( unsigned char key, int x, int y )
{
  switch (key) {
  case 27:
    exit(0);
    break;
  case '1': case '2': case '3': case '4': case '5':
    manipulation = Manip(static_cast<int>(key-'1'));
    break;
  case '<':
    if(tool_size > 0) --tool_size;
    break;
  case '>':
    if(tool_size < 30) ++tool_size;
    break;
  case '+':
    if(iso<255) ++iso;
    break;
  case '-': 
    if(iso>0) --iso;
    break;
  case 32:
    pan = !pan;
    break;
  case 'm':
    tex_vol->run_marching_cubes(iso);
    marching = true;
    break;
  case 't':
    marching = false;
    break;
  case 'd':
    cout << "iso value :" << iso << endl;
    cout << "pan mode :" << pan << endl;
    cout << "Tool no :" << manipulation << endl;
    cout << "Tool size :" << tool_size << endl;
    cout << "marching :" << marching << endl;
    break;
  }
  glutPostRedisplay();
}

void mouse_move_fun(int mousex, int mousey)
{
  Vec2i mouse_pos(mousex, mousey);
  
  if(left_mouse_button_down)
    sculpt(mouse_pos);
  else if(ball->get_current_action()!=NO_ACTION)
    ball->roll_ball(mouse_pos);
  glutPostRedisplay();
}

void mouse_button_fun(int button, int state, int mousex, int mousey)
{
  Vec2i mouse_pos(mousex, mousey);
  
  if(state == GLUT_DOWN)
    switch(button)
      {
      case GLUT_LEFT_BUTTON: 
				sculpt(mouse_pos);
				left_mouse_button_down = true;
				break;
      case GLUT_MIDDLE_BUTTON: 
				if(pan)
					ball->grab_ball(PAN_ACTION,mouse_pos);
				else 
					ball->grab_ball(ZOOM_ACTION,mouse_pos);
				break;
      case GLUT_RIGHT_BUTTON: 
				ball->grab_ball(ROTATE_ACTION,mouse_pos);
				break;
      }
  else
    {
      if(button==GLUT_LEFT_BUTTON) 
				{
					left_mouse_button_down = false;
					tex_vol->gl_bind_textures();
				}
      ball->release_ball();
    }
  glutPostRedisplay();
}

static void reshape( int width, int height )
{
  glViewport( 0, 0, width, height );
  ball->set_screen_window(width,height);
  ball->set_screen_centre(Vec2i(width/2,height/2));
  glutPostRedisplay();
}

bool check_dim(int& dim)
{
  if (dim < 2) 
    return false;
  static int powers2[9] = {2,4,8,16,32,64,128,256,512};
  for(int i=0;i<9;++i)
    {
      if(powers2[i]==dim) return true;
      if(powers2[i]>dim) 
				{
					dim=powers2[i];
					return false;
				}
    }
  return false;
}

int main(int argc, char *argv[])
{
  
  if(argc>1)
    {
      ResourceLoader res(argv[1]);
      const string name = res.getString("file_name");
      Vec3i voldim(res.getInt("slice_width"),
									 res.getInt("slice_height"),
									 res.getInt("slices"));
      Vec3f slice_scale(res.getFloat("x_scaling"),
												res.getFloat("y_scaling"),
												res.getFloat("z_scaling"));
      
      if(!check_dim(voldim[0]))
				{
					cout << "Illegal x dim of volume" << endl;
					exit(1);
				}
      if(!check_dim(voldim[1]))
				{
					cout << "Illegal y dim of volume" << endl;
					exit(1);
				}
      if(!check_dim(voldim[2]))
				cout << "Adding slices to pad volume" << endl;
      
      tex_vol = new TextureVolume(voldim[0],voldim[1],voldim[2],
																	slice_scale, name);
      ball = new TrackBall(tex_vol->vol_centre(),voldim[0],voldim[0],WINX,WINY);
      glutInit( &argc, argv );
      glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
      glutInitWindowSize( 500 , 500 );
      glutCreateWindow(argv[0]);
      glClearColor(0,.7,.7,1);
      reset_proj(voldim[0]*3);
      
      tex_vol->gl_init();
      
      glutReshapeFunc( reshape );
      glutKeyboardFunc( key );
      glutIdleFunc( idle );
      glutDisplayFunc( display );
      glutMouseFunc( mouse_button_fun );
      glutMotionFunc( mouse_move_fun );
      
      glutMainLoop();
      exit(0);
    }
  
  cout << "You must specify resource file" << endl;
  return 1;
}

