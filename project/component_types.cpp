#include "component_types.h"
#include <GL/glut.h>
#include <math.h>

void draw_capacitor(component_t comp)
{
    glBegin(GL_LINE_STRIP);
        glVertex2f(-13.0, 0.0);
        glVertex2f(-1.0, 0.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(-1.0, 5.0);
        glVertex2f(-1.0, -5.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(1.0, 5.0);
        glVertex2f(1.0, -5.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(13.0, 0.0);
        glVertex2f(1.0, 0.0);
    glEnd();
}

void draw_resistor(component_t comp)
{
    glBegin(GL_LINE_STRIP);
        glVertex2f(-13.0, 0.0);
        glVertex2f(-10.0, 0.0);
        glVertex2f(-8.0, 4.0);
        glVertex2f(-4.0, -4.0);
        glVertex2f(0.0, 4.0);
        glVertex2f(4.0, -4.0);
        glVertex2f(8.0, 4.0);
        glVertex2f(10.0, 0.0);
        glVertex2f(13.0, 0.0);
    glEnd();
}

void draw_transistor(component_t comp)
{
    int radius = 8;

    glBegin(GL_LINE_STRIP);
        for(float angle = 0; angle <= 360.0; angle = angle + 0.1) {
            glVertex2f(radius * cos(angle),
                       radius * sin(angle));
        }
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(-10.0, 0.0);
        glVertex2f(-2.0, 0.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(-2.0, 5.0);
        glVertex2f(-2.0, -5.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(4.0, 12.0);
        glVertex2f(4.0, 5.0);
        glVertex2f(-2.0, 2.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(4.0, -12.0);
        glVertex2f(4.0, -5.0);
        glVertex2f(-2.0, -2.0);
    glEnd();
}

void draw_wire(component_t comp)
{
  int radius = 2;

  glBegin(GL_LINE_STRIP);
    for(float angle = 0; angle <= 360.0; angle = angle + 0.1) {
      glVertex2f(radius * cos(angle) - 5.0,
                 radius * sin(angle) - 0.0);
    }
  glEnd();
  
  glBegin(GL_LINE_STRIP);
    glVertex2f(-5.0, 0.0);
    glVertex2f(5.0, 0.0);
  glEnd();

  glBegin(GL_LINE_STRIP);
    for(float angle = 0; angle <= 360.0; angle = angle + 0.1) {
      glVertex2f(radius * cos(angle) + 5.0,
                 radius * sin(angle) + 0.0);
    }
  glEnd();
}