#include "glCanvas.h"
#include <iostream> 
#include <ctime> 
#include <cstdlib>

#include "argparser.h"
#include "mesh.h"

// =========================================
// =========================================

int main(int argc, char *argv[]) {
  ArgParser *args = new ArgParser(argc, argv);
  Mesh *mesh = new Mesh(args);
  mesh->Load();
  mesh->ComputeGouraudNormals();

  glutInit(&argc,argv);
  GLCanvas::initialize(args,mesh); 

  delete mesh;
  delete args;
  return 0;
}

// =========================================
// =========================================
