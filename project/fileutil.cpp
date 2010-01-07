#include "fileutil.h"

void save_file(char *filename,
               std::vector<component_t> *components,
               program_settings_t *settings) {

                 FILE *file;

                 file = fopen(filename, "wb");

                 fprintf(file, "%d %d %f %d %d\n",
                   settings->x_displ, settings->y_displ,
                   settings->zoom,
                   settings->width,
                   settings->height);


                 std::vector<component_t>::iterator compiter;
                 for (compiter = components->begin();
                   compiter != components->end();
                   compiter++) {

                     switch (compiter->type) {
           case wire:
             fprintf(file, "%d %d %d %d %d %d %d\n",
               compiter->type,
               compiter->tx, compiter->ty,
               compiter->x1, compiter->y1,
               compiter->x2, compiter->y2);
             break;
           default:
             fprintf(file, "%d %d %d %d %f %f\n",
               compiter->type,
               compiter->tx, compiter->ty,
               compiter->rx,
               compiter->sx, compiter->sy);
                     }

                 }

                 fclose(file);

}