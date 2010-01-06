#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include "components.h"
#include "settings.h"
#include <GL/glut.h>

void draw_capacitor(component_t comp, int i);
void draw_resistor(component_t comp, int i);
void draw_transistor(component_t comp, int i);
void draw_wire(component_t comp, int i);

#endif