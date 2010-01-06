#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <vector>
#include "components.h"

/* FILE FORMAT:
 * Header:
 * <int x_displacement> <int y_displacement> <float zoom> <int width> <int height>
 * 
 * Followed by zero or more lines of the following format:
 * <int type> <int x> <int y> <int rotation> <float scalex> <float scaley>
 */
void save_components(char *filename, std::vector<component_t> *components);

#endif