#include "GL/glew.h"

#define CHECKGL( GLcall )                    \
{                                            \
GLcall;                                      \
if(1)                                        \
check_gl_error( #GLcall, __FILE__, __LINE__ ); \
}

void* load_ppm(const char* fn, int& width, int& height);

const char* load_file(const char* filename);

void check_gl_error(const char *GLcall, const char *file, int line);

GLuint create_shader(GLenum type, const char* fn);
GLuint create_program(const char* vs, const char* fs);

//awk 10/11/08