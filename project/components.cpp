#include "components.h"

component_t comp_parse_line(char *line) {
  component_t c;

  char *tok;
  tok = strtok(line, " ");
  c.type = atoi(tok);

  switch (c.type) {
    case wire:
      // do stuff:
      break;
    default:
      tok = strtok(NULL, " ");
      c.tx = atoi(tok);

      tok = strtok(NULL, " ");
      c.ty = atoi(tok);

      tok = strtok(NULL, " ");
      c.rx = atoi(tok);

      tok = strtok(NULL, " ");
      c.sx = atof(tok);

      tok = strtok(NULL, " ");
      c.sy = atof(tok);
  }

  return c;
}

void comp_parse_file(char *filename, std::vector<component_t> *components) {
  FILE *file = fopen(filename, "rb");

  char line[100];

  components->clear();

  fgets(line, sizeof(line), file);

  while(fgets (line, sizeof(line), file) != NULL) {
    components->push_back(comp_parse_line(line));
  }
  
  fclose (file);
}