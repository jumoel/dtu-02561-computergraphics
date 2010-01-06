#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <vector>
#include "components.h"
#include "settings.h"

/* FILE FORMAT:
 * Header:
 * <int x_displacement> <int y_displacement> <float zoom> <int width> <int height>
 * 
 * Followed by zero or more lines of the following format:
 * - transistors, resistors and capacitors:
 * <int type> <int x> <int y> <int rotation> <float scalex> <float scaley>
 *
 * - wires
 * <int type> <int x> <int y> <int x1> <int y1> <int x2> <int y2>
 */
void save_file(char *filename,
               std::vector<component_t> *components,
               program_settings_t *settings);

#endif