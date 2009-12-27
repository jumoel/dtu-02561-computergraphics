#ifndef __TEXTUREVOLUME_H
#define __TEXTUREVOLUME_H
// Author: J. Andreas Bærentzen,
// Created: Tue Nov  6 10:00:4

#include <string>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>


#include "CGLA/Vec3f.h"
#include "CGLA/Vec3i.h"
#include "TexSlice.h"

/** This enum defines five manipulations. */
enum Manip
	{
		PERLIN_ADD, PERLIN_SUB, MAX_ADD, MIN_SUB, ERASE
	};

/** The Texture Volume Class. This class contains information
		about a volume and stores a texture representation of the volume:
		This representation is simply the volume stored as slices perpendicular
		to each of the three major axes (x, y, and z). 

		The class contains function for:
		1. Primitive editing of the volume, i.e. getting and 
		changing voxel values.
		2. rendering the volume using texture mapping,
		3. Editing the volume (cut out a chunk add/remove spherical shape).
		4. Marching cubes polygonization and polygon rendering. 
*/
class TextureVolume
{
	/// Size of volume in x-axis direction
	const int XSIZE;
	/// Size of volume in y-axis direction
	const int YSIZE;
	/// Size of volume in z-axis direction (n.o. slices)
	const int SLICES;

	/// 3d vector containing the scaling factor for each of (x,y,z)
	const CGLA::Vec3f slice_scale;

	/// Volume dimensions
	const float minX, minY, minZ, maxX, maxY, maxZ;

	/// Texture names for slices along x direction
	std::vector<int> x_slice_textures;
	std::vector<TexSlice> x_data;
	
	/// Texture names for slices along y direction
	std::vector<int> y_slice_textures;
	std::vector<TexSlice> y_data;
	
	/// Texture names for slices along z direction
	std::vector<int> z_slice_textures;
	std::vector<TexSlice> z_data;

	/// Name of volume disk file
	const std::string name;

	/// Load the volume
	void load();

	/// Bind texture and draw z-slice 
	void draw_z_slice(float, int);

	/// Bind texture and draw y-slice 
	void draw_y_slice(float, int);

	/// Bind texture and draw x-slice 
	void draw_x_slice(float, int);

public:

	/** Construct a texture volume. The first three parameters are the
			dimensions of the volume. The fourth parameter is a vector containing
			the scaling factors for each slice direction. The fifth argument is
			the name of the file containing the volume. */
 	TextureVolume(int _XSIZE, int _YSIZE, int _SLICES, 
								const CGLA::Vec3f _slice_scale,
								const std::string& _name);

	/// Returns a vector to the centre of the volume.
	CGLA::Vec3f vol_centre() const {return CGLA::Vec3f(maxX,maxY,maxZ)/2.0f;}

	/** Function that verifies whether a point is inside the volume.
			The parameter is a 3D int vector. The function returns true
			if the parameter corresponds to a voxel position. In practical
			terms, the function returns true if get and set will succeed
			given the same parameter. */
 	bool point_in_vol(const CGLA::Vec3i& p)
	{
		return p.all_ge(CGLA::Vec3i(0,0,0)) && p.all_l(CGLA::Vec3i(XSIZE,YSIZE,SLICES));
	}

	/** Get voxel value. This function accepts a 3d int vector as parameter
			and returns a Byte (unsigned char). The parameter is a voxel
			position and the return value is the corresponding voxel value. */
 	Byte get(const CGLA::Vec3i&p) const {return z_data[p[2]].get(p[0],p[1]);}

	/** Set voxel value. This function takes two parameters: a 3d int vector
			and an unsigned char value. These parameters correspond to a voxel 
			position and a voxel value. The voxel at the position given as first
			argument is assigned the value given as second argument. */
	void set(const CGLA::Vec3i& p, Byte val)
	{
		x_data[p[0]].get(p[1],p[2]) = 
			y_data[p[1]].get(p[2],p[0]) = 
			z_data[p[2]].get(p[0],p[1]) = val;
	}

	/** Draw a stack of volume slices. The function accepts two parameters,
			an 3D floating point vector (eye direction)  and a float (the iso value) 
			The eye direction determines which	of the three slice directions 
			(x,y, or z) that is used, and the iso value determines the threshold 
			for which intensities that are drawn */
	void gl_draw_stack(const CGLA::Vec3f& eye, float iso);

	/** Set up OpenGL state and generate texture names */
 	void gl_init();

	/** Each of the generated textures is bound, and teximage2d is
			used to assign a slice image to that texture */ 
 	void gl_bind_textures();

	/** Draw marching cubes polygons (not part of exercise) */
	void gl_draw_polygons();

	/** Manipulate volume. This function accepts three parameters.
			The first parameter is a 3D float vector, the position.
			The second parameter is an integer, the size, and the third
			parameter is of an enumerated type specifying which manipulation
			that should take place. */
 	void manipulate(const CGLA::Vec3f p, int N, Manip);

	/** Saves volume. This simply writes the volume back to disk. */
 	void save();

	/// Compute gradient using central differences.
	CGLA::Vec3f grad(const CGLA::Vec3i& p) const;

	/// Run marching cubes algorithm.
	void run_marching_cubes(Byte iso_value);
};

inline CGLA::Vec3f TextureVolume::grad(const CGLA::Vec3i& p) const
{
	static const CGLA::Vec3i dx = CGLA::Vec3i(1,0,0);
	static const CGLA::Vec3i dy = CGLA::Vec3i(0,1,0);
	static const CGLA::Vec3i dz = CGLA::Vec3i(0,0,1);
	return CGLA::Vec3f((float(get(p+dx))-float(get(p-dx)))/2.0f,
							 (float(get(p+dy))-float(get(p-dy)))/2.0f,
							 (float(get(p+dz))-float(get(p-dz)))/2.0f);
}


inline void TextureVolume::draw_z_slice(float z, int i)
{}

inline void TextureVolume::draw_y_slice(float y, int i)
{}

inline void TextureVolume::draw_x_slice(float x, int i)
{
}



#endif
