#include "component_types.h"
#include <GL/glut.h>
#include <math.h>

#define LINE_A (INDEX) { INDEX ## 001 }
#define LINE_B (INDEX) { INDEX ## 002 }

void draw_capacitor(component_t comp, int i)
{
  glPushName(i);
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
  glPopName();
}

void draw_resistor(component_t comp, int i)
{
  glPushName(i);
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
  glPopName();
}

void draw_transistor(component_t comp, int i)
{
  glPushName(i);
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
  glPopName();
}

void draw_wire(component_t comp, int i)
{
  int radius = 1;

  glPushName(1000 + i);

  glBegin(GL_LINE_STRIP);
    for(float angle = 0; angle <= 360.0; angle = angle + 0.1) {
      glVertex2f(radius * cos(angle) + comp.x1,
                 radius * sin(angle) + comp.y1);
    }
  glEnd();

  glPopName();

  glPushName(2000 + i);

  glBegin(GL_LINE_STRIP);
    for(float angle = 0; angle <= 360.0; angle = angle + 0.1) {
      glVertex2f(radius * cos(angle) + comp.x2,
                 radius * sin(angle) + comp.y2);
    }
  glEnd();

  glPopName();

  glPushName(i);

  glBegin(GL_LINE_STRIP);
    glVertex2f(comp.x1, comp.y1);
    glVertex2f(comp.x2, comp.y2);
  glEnd();

  glPopName();
}