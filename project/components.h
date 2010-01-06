#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <vector>
#include <stdio.h>

enum component_type
{
	capacitor,
	resistor,
	transistor
};

struct component_t
{
	component_t(int t=0, int x=0, int y=0) : type(t), tx(x), ty(y), rx(0), sx(1), sy(1) {}

	int type;		//type of component
	int tx, ty;		//translation
	int rx;			//rotation in degrees
	float sx, sy;	//scale
};

component_t comp_parse_line(char *line);
void comp_parse_file(char *filename, std::vector<component_t> *components);

#endif