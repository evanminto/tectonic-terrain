#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>

class ArgParser {

public:

  ArgParser() { DefaultValues(); }

  ArgParser(int argc, char *argv[]) {
    DefaultValues();
    // parse the command line arguments
    for (int i = 1; i < argc; i++) {
      if (!strcmp(argv[i],"-input") ||
	  !strcmp(argv[i],"-i")) {
	i++; assert (i < argc); 
	input_file = std::string(argv[i]);
      } else if (!strcmp(argv[i],"-vertex") ||
		 !strcmp(argv[i],"-v")) {
	i++; assert (i < argc); 
	vertex_shader_filename = std::string(argv[i]);
      } else if (!strcmp(argv[i],"-fragment") ||
		 !strcmp(argv[i],"-f")) {
	i++; assert (i < argc); 
	fragment_shader_filename = std::string(argv[i]);
      } else if (!strcmp(argv[i],"-heightmap") ||
		 !strcmp(argv[i],"-h")) {
	i++; assert (i < argc); 
	heightmap_filename = std::string(argv[i]);
      } else if (!strcmp(argv[i],"-normal") ||
     !strcmp(argv[i],"-n")) {
  i++; assert (i < argc); 
  normalmap_filename = std::string(argv[i]);
      } else if (!strcmp(argv[i],"-preset") ||
     !strcmp(argv[i],"-p")) {
  i++; assert (i < argc); 
  preset_config = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-size") ||
                 !strcmp(argv[i],"-s")) {
	i++; assert (i < argc); 
	width = height = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-t")) {
        i++; assert (i < argc);
        timestep = atof(argv[i]);
      } else {
	printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
	assert(0);
      }
    }
  }

  void DefaultValues() {
    input_file = "flat16.obj";
    vertex_shader_filename = "ter.vs";
    fragment_shader_filename = "ter.fs";
    heightmap_filename = "flat.bmp";
    normalmap_filename = "flat.bmp";
    width = 600;
    height = 600;
    glsl_enabled = false;
    glsl_initialized = false;
    timer = 0.0;
    timestep = 200.0; // ms
    preset = 0;
  }

  // ==============
  // REPRESENTATION
  // all public! (no accessors)

  std::string input_file;
  std::string fragment_shader_filename;
  std::string vertex_shader_filename;
  std::string heightmap_filename;
  std::string normalmap_filename;
  int width;
  int height;
  bool glsl_enabled;
  bool glsl_initialized;
  float timer;
  float timestep;
  int preset;
};

#endif
