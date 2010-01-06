#include "fileutil.h"

void save_components(char *filename, std::vector<component_t> *components) {
  FILE *file;
  
  file = fopen(filename, "wb");

  printf("Complength: %d\n", components->size());

  std::vector<component_t>::iterator compiter;
  for (compiter = components->begin();
       compiter != components->end();
       compiter++) {

         fprintf(file, "%d %d %d %d %f %f\n",
           compiter->type,
           compiter->tx, compiter->ty,
           compiter->rx,
           compiter->sx, compiter->sy);

  }

  fclose(file);

}