#ifndef __MARCHINGCUBES_H
#define __MARCHINGCUBES_H
// Author: J. Andreas Bærentzen,
// Created: Wed Aug  1 12:31:3
#include "CGLA/Vec3f.h"

extern const CGLA::Vec3i MCCornersi[8]; 
extern const CGLA::Vec3f MCCornersf[8];

int polygonize_cell(const CGLA::Vec3f& p0, 
										float isovalue,
										const float vox_values[8], 
										const CGLA::Vec3f vox_normals[8],
										std::vector<CGLA::Vec3f>& vertices,
										std::vector<CGLA::Vec3f>& normals);

#endif
