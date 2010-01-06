#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <vector>
#include "components.h"

/* FILE FORMAT:
 * <type> <x> <y> <rotation> <scalex> <scaley>
 */
void save_components(char *filename, std::vector<component_t> *components);

#endif