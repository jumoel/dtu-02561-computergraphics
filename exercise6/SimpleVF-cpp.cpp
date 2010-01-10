/* display teapot with vertex and fragment shaders */
/* sets up elapsed time parameter for use by shaders */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string.h>
#include <errno.h>


const GLdouble nearVal     = 1.0;
const GLdouble farVal      = 20.0;
const GLfloat  lightPos[4] = {3.0f, 3.0f, 3.0f, 1.0f};
GLuint         program     = 0;
GLint          timeParam;
static GLuint texture;
static GLint textureloc;

GLchar *ebuffer; /* buffer for error messages */
GLsizei elength; /* length of error message */

void* load_ppm(const char* fn, int &width, int &height)
{
    FILE* f = fopen(fn, "rb");
    if (!f) exit(-1);
    
    char magic[3];
    fread(magic, 1, 3, f);
    if (magic[0]!='P' || magic[1]!='6')
        exit(-1);

	char c;
	do
	{
		char tmp[256];
		fscanf(f, "%[^\n]", tmp);	
		fscanf(f, "%c", &c);
	}
	while (c == '#');
	ungetc(c,f);

    int max;
    fscanf(f, "%u\n%u\n%u\n", &width, &height, &max);

    void* rgb = malloc(width*height*3);
    fread(rgb, 3, width*height, f);
    fclose(f);

    return rgb;
}

/* shader reader */
/* creates null terminated string from file */
static char* readShaderSource(const char* shaderFile)
{
  struct stat statBuf;
  FILE *fp = fopen(shaderFile, "rb");

  if (fp == NULL) { printf("fp null"); printf("%s", strerror(errno)); getchar(); exit(EXIT_FAILURE); }

  char* buf;

  stat(shaderFile, &statBuf);
  buf = (char*) malloc((statBuf.st_size + 1) * sizeof(char));
  fread(buf, 1, statBuf.st_size, fp);
  buf[statBuf.st_size] = '\0';
  fclose(fp);
  return buf;
}

/* error printing function */
static void checkError(GLint status, const char *msg)
{
  if (!status)
  {
    printf("%s\n", msg);
    getchar();
    exit(EXIT_FAILURE);
  }
}

/*	Create checkerboard texture	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];
void makeCheckImage(void)
{
    int i, j, c;
    
    for (i = 0; i < checkImageWidth; i++) {
	    for (j = 0; j < checkImageHeight; j++) {
	        c = ((((i&0x8)==0)^((j&0x8))==0))*255;
	        checkImage[i][j][0] = (GLubyte) c;
	        checkImage[i][j][1] = (GLubyte) c;
	        checkImage[i][j][2] = (GLubyte) c;
	    }
    }
}

/* standard OpenGL initialization */
static void init()
{
  const float teapotColor[]     = {0.3f, 0.5f, 0.4f, 1.0f}; 
  const float teapotSpecular[]  = {0.8f, 0.8f, 0.8f, 1.0f};
  const float teapotShininess[] = {80.0f};

  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, teapotColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, teapotSpecular);
  glMaterialfv(GL_FRONT, GL_SHININESS, teapotShininess);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double) glutGet(GLUT_WINDOW_WIDTH) / (double) glutGet(GLUT_WINDOW_HEIGHT), nearVal, farVal);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHTING); 
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  
  makeCheckImage();
  glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, 
	                checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 
	                &checkImage[0][0][0]);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_TEXTURE_2D);
}

/* GLSL initialization */

static void initShader(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
  GLint status = glGetError()==GL_NO_ERROR; 
  GLchar *vSource, *fSource;
  GLuint vShader, fShader;

  /* read shader files */
  vSource = readShaderSource(vShaderFile);
  checkError(status, "Failed to read vertex shader");

  fSource = readShaderSource(fShaderFile);
  checkError(status, "Failed to read fragment shader");

  /* create program and shader objects */
  vShader = glCreateShader(GL_VERTEX_SHADER);
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  program = glCreateProgram();

  /* attach shaders to the program object */
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);

  /* read shaders */
  glShaderSource(vShader, 1, (const GLchar**) &vSource, NULL);
  glShaderSource(fShader, 1, (const GLchar**) &fSource, NULL);

  /* compile shaders */
  glCompileShader(vShader);
  glCompileShader(fShader);

  /* error check */
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
  checkError(status, "Failed to compile the vertex shader.");

  glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)malloc(len);	
		glGetShaderInfoLog(fShader, len, NULL, log); 
		printf(log); free(log); getchar(); exit(-1);
	}
  checkError(status, "Failed to compile the fragment shader.");

  /* link */
  glLinkProgram(program);
    
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  checkError(status, "Failed to link the shader program object.");
    
  /* use program object */
  glUseProgram(program);

  textureloc = glGetUniformLocation(program, "tex");
  glUniform1i(textureloc,0);

  /* set up uniform parameter */
  timeParam = glGetUniformLocation(program, "time");
}

static void draw(void)
{
  /* send elapsed time to shaders */
  glUniform1f(timeParam, glutGet(GLUT_ELAPSED_TIME));
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -5.0f);
  glutSolidTeapot(1.0);
  glPopMatrix();
  glutSwapBuffers();
}

static void reshape(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double) w / (double) h, nearVal, farVal);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0, 0, w, h);

  glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:
    case 'Q':
    case 'q':
      exit(EXIT_SUCCESS);
      break;
    default:
      break;
  }
}

static void idle(void) {
  glUniform1f(timeParam, glutGet(GLUT_ELAPSED_TIME));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(textureloc,0);

  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Simple GLSL example");
	glewInit ();
  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);

  init();
  initShader("vFragPhongTex.glsl", "fPhongTex.glsl");

  glutMainLoop();
  return 0;
}
