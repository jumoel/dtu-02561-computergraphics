#ifndef SETTINGS_H
#define SETTINGS_H

enum save_files
{
	file_autosave,
  file_manualsave
};

struct program_settings_t {
  program_settings_t(double z = 4.0,
                     int x = 0, int y = 0,
                     int h = 512, int w = 512) : 
                      zoom(z),
                      x_displ(x), y_displ(y),
                      height(h), width(w) {};
  
  double zoom;
  int x_displ;
  int y_displ;
  int height;
  int width;
};

void settings_parse_file(char *filename, program_settings_t *settings);

#endif