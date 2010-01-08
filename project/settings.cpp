#include "settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void settings_parse_file(char *filename, program_settings_t *settings) {
  FILE *file = fopen(filename, "rb");

  if (file == NULL) return;

  char line[100];

  if (fgets (line, sizeof(line), file) != NULL) {
    char *tok;

    tok = strtok(line, " "); if (tok == NULL) return;
    settings->x_displ = atoi(tok);

    tok = strtok(NULL, " "); if (tok == NULL) return;
    settings->y_displ = atoi(tok);

    tok = strtok(NULL, " "); if (tok == NULL) return;
    settings->zoom = atof(tok);

    tok = strtok(NULL, " "); if (tok == NULL) return;
    settings->width = atoi(tok);

    tok = strtok(NULL, " "); if (tok == NULL) return;
    settings->height = atoi(tok);
  }

  fclose (file);
}