// Author: J. Andreas Bærentzen,
// Created: Tue Nov  6 10:00:4

//#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <GL/glut.h>

#include "TextureVolume.h"
#include "MarchingCubes.h"

using namespace std;
using namespace CGLA;

TextureVolume::TextureVolume(int _XSIZE, int _YSIZE, int _SLICES, 
														 const Vec3f _slice_scale,
														 const string& _name): 
	XSIZE(_XSIZE), YSIZE(_YSIZE), SLICES(_SLICES), 
	slice_scale(_slice_scale),
	minX(0), minY(0), minZ(0), 
	maxX(XSIZE*slice_scale[0]),
	maxY(YSIZE*slice_scale[1]),
	maxZ(SLICES*slice_scale[2]),
	x_slice_textures(XSIZE,0),	x_data(XSIZE,TexSlice(YSIZE,SLICES)),
	y_slice_textures(YSIZE,0),	y_data(YSIZE,TexSlice(SLICES,XSIZE)),
	z_slice_textures(SLICES,0),	z_data(SLICES,TexSlice(XSIZE,YSIZE)),
	name(_name)
{
	load();
}

void TextureVolume::load()
{
#ifdef WIN32
	int f=open(name.c_str(),O_RDONLY|O_BINARY);
#else
	int f=open(name.c_str(),O_RDONLY);
#endif
	if(f==-1)
		{
			cout << "Could not open file" << endl;
			exit(1);
		}
	else
		for(int k=0;k<SLICES;++k)
			{
				if(read(f,&z_data[k].get(0,0),z_data[k].size()) != z_data[k].size())
					{
						cout << "Warning (probably harmless): Failed to read slice "
								 << k << endl;
						break;
					}
			}
	for(int k=0;k<SLICES;++k)
		for(int i=0;i<XSIZE;++i)
			for(int j=0;j<YSIZE;++j)
				x_data[i].get(j,k) = y_data[j].get(k,i) = z_data[k].get(i,j);
 
}

void TextureVolume::save()
{
	int f=open(name.c_str(),O_WRONLY);
	if(f==-1)
		{
			cout << "Could not open file" << endl;
			exit(1);
		}
	else
		for(int k=0;k<SLICES;++k)
			{
				if(write(f,&z_data[k].get(0,0),z_data[k].size())==-1)
					{
						cout << "Warning: Failed to write slice " << k << endl;
						break;
					}
			}
}


void  TextureVolume::manipulate(const Vec3f p, int N, Manip man)
{}

void TextureVolume::gl_draw_stack(const Vec3f& eye, float iso)
{
}

void TextureVolume::gl_init() 
{
	gl_bind_textures();
}


void TextureVolume::gl_bind_textures()
{
}


void  TextureVolume::gl_draw_polygons()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	
	glCullFace(GL_FRONT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

	GLfloat matAmb[4]  = { 0.1f, 0.2f , 0.4f, .5f};
	GLfloat matDiff[4] = { 0.1f, 0.4f , 0.8f, 0.5f};
	GLfloat matSpec[4] = { 0.1f, 0.4f , 0.8f, 0.5f};
	GLfloat matShine = 50;
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShine);

	glPushMatrix();
	glScalef(slice_scale[0],slice_scale[1],slice_scale[2]);
	glCallList(1);
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

}

void TextureVolume::run_marching_cubes(Byte iso_value)
{
	glNewList(1, GL_COMPILE);
	for(int k=1;k<SLICES-2;++k)
		{
			for(int i=1;i<XSIZE-2;++i)
				for(int j=1;j<YSIZE-2;++j)
					{
						float voxels[8];
						Vec3f grads[8];
						
						Vec3i pi(i,j,k);
						Vec3f pf(pi);

						bool above=false;
						bool below=false;
						for(int l=0;l<8;++l)
							{
								voxels[l] = float(get(pi+MCCornersi[l]));
								if(voxels[l]>=iso_value) above = true;
								if(voxels[l]<=iso_value) below = true;
							}
						if(above&&below)
							{
								std::vector<CGLA::Vec3f> mc_vertices;
								std::vector<CGLA::Vec3f> mc_normals;

								mc_vertices.reserve(20);
								mc_normals.reserve(20);

								for(int l=0;l<8;++l)
									grads[l] = - normalize(grad(pi+MCCornersi[l]));
								polygonize_cell(pf, iso_value,
																voxels, grads,
																mc_vertices, mc_normals);
								
								glBegin(GL_TRIANGLES);
								for(int i=0;i<mc_vertices.size();++i)
									{
										glNormal3fv(mc_normals[i].get());
										glVertex3fv(mc_vertices[i].get());
									}
								glEnd();
							}
					}
			cout << "Processed slice " << k << endl;
		}
	glEndList();
}
