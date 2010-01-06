#include "settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void settings_parse_file(char *filename, program_settings_t *settings) {
  FILE *file = fopen(filename, "rb");

  char line[100];

  if (fgets (line, sizeof(line), file) != NULL) {
    char *tok;
    tok = strtok(line, " ");
    settings->x_displ = atoi(tok);

    tok = strtok(NULL, " ");
    settings->y_displ = atoi(tok);

    tok = strtok(NULL, " ");
    settings->zoom = atof(tok);

    tok = strtok(NULL, " ");
    settings->width = atoi(tok);

    tok = strtok(NULL, " ");
    settings->height = atoi(tok);
  }

  fclose (file);
}