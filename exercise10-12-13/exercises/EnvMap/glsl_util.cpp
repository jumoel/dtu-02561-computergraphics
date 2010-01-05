#include "glsl_util.hpp"

#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#pragma warning (disable : 4996) 
 
void* load_ppm(const char* fn, int& width, int& height)
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
		char tmp[128];
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


void check_gl_error(void)
{
    GLenum err = glGetError();

    if (err == GL_NO_ERROR)
        return;

    printf("ERROR: %s\n", gluErrorString(err));
    getchar(); // Let me read the error, goddamnit
    exit(-1);
}

const char* load_file(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	assert(f);
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);	
	char* d = (char*)malloc(size+1);
	fread(d, 1, size, f);
	d[size] = '\0';
	return d;
}

GLuint create_shader(GLenum type, const char* fn)
{
	const char* src = load_file(fn);
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, &src, NULL);
	free((char*)src);
	glCompileShader(handle);	
	int compiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled); 
	if (compiled == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)malloc(len);	
		glGetShaderInfoLog(handle, len, NULL, log); 
		printf(log); free(log); getchar(); exit(-1);
	}
	return handle;
}

GLuint create_program(const char* vs, const char* fs)
{
	GLuint prg = glCreateProgram();	
	if (vs)
	{
		GLuint vh = create_shader(GL_VERTEX_SHADER, vs);
		glAttachShader(prg, vh);
	}
	if (fs)
	{
		GLuint fh = create_shader(GL_FRAGMENT_SHADER, fs);
		glAttachShader(prg, fh);
	}
	glLinkProgram(prg);	
	int linked;
	glGetProgramiv(prg, GL_LINK_STATUS, &linked); 
	if (linked == GL_FALSE)
	{
		int len;
		glGetProgramiv(prg, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)malloc(len);	
		glGetProgramInfoLog(prg, len, NULL, log); 
		printf(log); free(log); getchar(); exit(-1);
	}
	return prg;
}

//awk 10/11/08