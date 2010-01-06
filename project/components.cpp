#include "components.h"

component_t comp_parse_line(char *line) {
  component_t c;

  char *tok;
  tok = strtok(line, " ");
  c.type = atoi(tok);

  switch (c.type) {
    case wire:
      tok = strtok(NULL, " ");
      c.tx = atoi(tok);

      tok = strtok(NULL, " ");
      c.ty = atoi(tok);

      tok = strtok(NULL, " ");
      c.x1 = atoi(tok);

      tok = strtok(NULL, " ");
      c.y1 = atoi(tok);

      tok = strtok(NULL, " ");
      c.x2 = atoi(tok);

      tok = strtok(NULL, " ");
      c.y2 = atoi(tok);
      break;
    default:
      tok = strtok(NULL, " ");
      c.tx = atoi(tok);

      tok = strtok(NULL, " ");
      c.ty = atoi(tok);

      tok = strtok(NULL, " ");
      c.rx = atoi(tok);

      tok = strtok(NULL, " ");
      c.sx = (float)atof(tok);

      tok = strtok(NULL, " ");
      c.sy = (float)atof(tok);
  }

  return c;
}

void comp_parse_file(char *filename, std::vector<component_t> *components) {
  FILE *file = fopen(filename, "rb");

  char line[100];

  components->clear();

  if (fgets(line, sizeof(line), file) != NULL) {
    while(fgets (line, sizeof(line), file) != NULL) {
      components->push_back(comp_parse_line(line));
    }
  }
  
  fclose (file);
}