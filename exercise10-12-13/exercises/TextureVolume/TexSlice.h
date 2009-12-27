#ifndef __TEXSLICE_H
#define __TEXSLICE_H
// Author: J. Andreas Bærentzen,
// Created: Tue Nov  6 09:59:2

#include <vector>
typedef unsigned char Byte;


/** This class represents a slice of a 3D texture. Each texel is (so far)
		assumed to be just one Byte=unsigned char.
*/
class TexSlice
{
	/// Dimensions of slice
	const int XSIZE,YSIZE;

	/// Size of slice (number of bytes)
	const int SIZE;

	/// Vector containing actual data.
	std::vector<Byte> data;

public:
	
	/** Construct a slice. */
	TexSlice(int _XSIZE, int _YSIZE): 
		XSIZE(_XSIZE), YSIZE(_YSIZE), SIZE(XSIZE*YSIZE), 
		data(SIZE,0) {}

	/** Get a pixel. Get accepts two parameters, the x and y pixel positions
			and returns a reference to the pixel at that position */
 	Byte& get(int i, int j) {return data[j*XSIZE+i];}

	/// This is the const version of get 
	const Byte& get(int i, int j) const {return data[j*XSIZE+i];}

	/// Returns the size of the slice.
	int size() const {return SIZE;}
};

#endif
