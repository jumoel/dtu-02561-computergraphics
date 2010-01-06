#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <vector>
#include <stdio.h>

enum component_type
{
	capacitor,
	resistor,
	transistor,
  wire
};

struct component_t
{
	component_t(int t=0, int x=0, int y=0) : 
              type(t),
              tx(x), ty(y),
              rx(0),
              sx(1), sy(1),
              x1(1), y1(0),
              x2(1), y2(0) {}

	int type;		//type of component
	int tx, ty;		//translation
  int x1, y1; // start points for wires
  int x2, y2; // end points for wires
	int rx;			//rotation in degrees
	float sx, sy;	//scale
};

component_t comp_parse_line(char *line);
void comp_parse_file(char *filename, std::vector<component_t> *components);

#endif