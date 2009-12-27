#include "GL/glew.h"

void* load_ppm(const char* fn, int& width, int& height);

const char* load_file(const char* filename);

void check_gl_error(void);

GLuint create_shader(GLenum type, const char* fn);
GLuint create_program(const char* vs, const char* fs);

//awk 10/11/08