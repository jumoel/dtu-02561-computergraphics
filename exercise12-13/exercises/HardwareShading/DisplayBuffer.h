#include <GL/glut.h>

class DisplayBuffer {
	GLfloat *data;
	GLubyte *stencildata;
	int size_x, size_y;
public:
	DisplayBuffer(int size_x, int size_y);
	void drawDepthBuffer();
	void drawStencilBuffer();
	void drawAlphaBuffer();
};

