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
      } else if (!strcmp(argv[i],"-size")) {
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
    input_file = "";
    vertex_shader_filename = "";
    fragment_shader_filename = "";
    width = 600;
    height = 600;
    mirror = false;
    shadow = false;
    geometry = true;
    reflected_geometry = false;
    bounding_box = false;
    silhouette_edges = false;
    shadow_polygons = false;
    gouraud_normals = false;
    glsl_enabled = false;
    glsl_initialized = false;
    timer = 0.0;
    timestep = 200.0; // ms
  }

  // ==============
  // REPRESENTATION
  // all public! (no accessors)

  std::string input_file;
  std::string fragment_shader_filename;
  std::string vertex_shader_filename;
  int width;
  int height;
  bool shadow;
  bool mirror;
  bool geometry;
  bool reflected_geometry;
  bool bounding_box;
  bool silhouette_edges;
  bool shadow_polygons;
  bool gouraud_normals;
  bool glsl_enabled;
  bool glsl_initialized;
  float timer;
  float timestep;
};

#endif
