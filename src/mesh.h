#ifndef MESH_H
#define MESH_H

#include "glCanvas.h"
#include <vector>
#include <string>
#include "vectors.h"
#include "hash.h"
#include "boundingbox.h"
#include "vbo_structs.h"
#include "Simulation.h"

class ArgParser;
class Vertex;
class Edge;
class Triangle;

// ======================================================================
// ======================================================================

class Mesh {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Mesh(ArgParser *_args);
  ~Mesh();
  void Load(); 
  void ComputeGouraudNormals();

  void initializeVBOs(); 
  void setupVBOs(); 
  void drawVBOs();
  void cleanupVBOs();
    
  // ========
  // VERTICES
  int numVertices() const { return vertices.size(); }
  Vertex* addVertex(const Vec3f &pos);
  // look up vertex by index from original .obj file
  Vertex* getVertex(int i) const {
    assert (i >= 0 && i < numVertices());
    Vertex *v = vertices[i];
    assert (v != NULL);
    return v; }
  void displaceVertices();

  // =====
  // EDGES
  int numEdges() const { return edges.size(); }
  // this efficiently looks for an edge with the given vertices, using a hash table
  Edge* getMeshEdge(Vertex *a, Vertex *b) const;

  // =========
  // TRIANGLES
  int numTriangles() const { return triangles.size(); }
  void addTriangle(Vertex *a, Vertex *b, Vertex *c);
  void removeTriangle(Triangle *t);

  // ===============
  // OTHER ACCESSORS
  const BoundingBox& getBoundingBox() const { return bbox; }
  Vec3f LightPosition();

private:

  // HELPER FUNCTIONS FOR PAINT
  void SetupLight(Vec3f light_position);
  void SetupMesh();
  void SetupPlateVisualization();

  void DrawLight();
  void DrawMesh();
  void DrawPlateVisualization();

  void TextureInit();

  // ==============
  // REPRESENTATION
  ArgParser *args;
  std::vector<Vertex*> vertices;
  edgeshashtype edges;
  triangleshashtype triangles;
  BoundingBox bbox;
  Simulation* simulation;

  // VBOs
  GLuint mesh_tri_verts_VBO;
  GLuint mesh_plate_verts_VBO;
  GLuint light_vert_VBO;

  std::vector<VBOPosNormalColorTexture> mesh_tri_verts; 
  std::vector<VBOPos> mesh_plate_verts; 
  std::vector<VBOPos> light_vert;
};

Vec3f ComputeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3);

// ======================================================================
// ======================================================================

#endif




