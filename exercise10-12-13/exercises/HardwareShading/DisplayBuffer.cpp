// bdl 13. nov 2001
#pragma warning (disable : 4786) 

#include "DisplayBuffer.h"
#include <iostream>

using namespace std;

DisplayBuffer::DisplayBuffer(int _size_x, int _size_y) : size_x(_size_x), size_y(_size_y) {
	data = new GLfloat[size_x*size_y*4];
	stencildata = new GLubyte[size_x*size_y];
}

//
// draw reads a buffer and draws it on the screen
// buffer can be one of the following
// GL_COLOR_INDEX, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and GL_LUMINANCE_ALPHA
//
void DisplayBuffer::drawDepthBuffer() {
	// Læs buffer
	int i;
	GLfloat min_value1=1;
	GLfloat max_value1=0;
	glReadPixels(0, 0, size_x, size_y, GL_DEPTH_COMPONENT, GL_FLOAT, data);
	for (i=0;i<(size_x)*(size_y);i++) {
		if (data[i]<min_value1) {
			min_value1=data[i];
		}
		if (data[i]>max_value1)
			max_value1=data[i];
	}
	float factor = 1;	factor=(float)1/(float)(max_value1-min_value1);
	for (i=0;i<size_x*size_y;i++)
		data[i] = (data[i]-min_value1)*factor;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,512,0,512);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(0, 0);
	glDrawPixels(size_x, size_y, GL_LUMINANCE , GL_FLOAT, data);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void DisplayBuffer::drawStencilBuffer() {
	int i;
	GLubyte min_value1=255;
	GLubyte max_value1=0;
	glReadPixels(0, 0, size_x, size_y, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencildata);
	for (i=0;i<(size_x)*(size_y);i++) {
		if (stencildata[i]<min_value1) {
			min_value1=stencildata[i];
		}
		if (stencildata[i]>max_value1)
			max_value1=stencildata[i];
	}
	float factor = 255/(float)(max_value1-min_value1);
	for (i=0;i<size_x*size_y;i++)
		stencildata[i] = (stencildata[i]-min_value1)*factor;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,512,0,512);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(0,0);
	glDrawPixels(size_x, size_y, GL_LUMINANCE , GL_UNSIGNED_BYTE, stencildata);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

void DisplayBuffer::drawAlphaBuffer() {
	int i;
	GLubyte min_value1=255;
	GLubyte max_value1=0;
	glReadPixels(0, 0, size_x, size_y, GL_ALPHA, GL_UNSIGNED_BYTE, stencildata);
	for (i=0;i<(size_x)*(size_y);i++) {
		if (stencildata[i]<min_value1) {
			min_value1=stencildata[i];
//			cout << "Found in " << i << endl;
		}
		if (stencildata[i]>max_value1)
			max_value1=stencildata[i];
	}
	float factor = 255/(float)(max_value1-min_value1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,512,0,512);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(0, 0);
	glDrawPixels(size_x, size_y, GL_LUMINANCE , GL_UNSIGNED_BYTE, stencildata);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}


