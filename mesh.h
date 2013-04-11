#ifndef MESH_H
#define MESH_H

#include "glCanvas.h"
#include <vector>
#include <string>
#include "vectors.h"
#include "hash.h"
#include "boundingbox.h"
#include "vbo_structs.h"

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
  Mesh(ArgParser *_args) { args = _args; }
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
  void SetupMirror();
  void SetupFloor();
  void SetupMesh();
  void SetupReflectedMesh();
  void SetupReflectedFloor();
  void SetupSilhouetteEdges(Vec3f light_position);
  void SetupShadowPolygons(Vec3f light_position);

  void DrawLight();
  void DrawMirror();
  void DrawFloor();
  void DrawMesh();
  void DrawReflectedMesh();
  void DrawReflectedFloor();
  void DrawSilhouetteEdges();
  void DrawShadowPolygons();

  // ==============
  // REPRESENTATION
  ArgParser *args;
  std::vector<Vertex*> vertices;
  edgeshashtype edges;
  triangleshashtype triangles;
  BoundingBox bbox;

  // VBOs
  GLuint mesh_tri_verts_VBO;
  GLuint reflected_mesh_tri_verts_VBO;
  GLuint shadow_polygon_quad_verts_VBO;
  GLuint light_vert_VBO;
  GLuint mirror_quad_verts_VBO;
  GLuint floor_quad_verts_VBO;
  GLuint reflected_floor_quad_verts_VBO;
  GLuint silhouette_edge_verts_VBO;

  std::vector<VBOPosNormal> mesh_tri_verts; 
  std::vector<VBOPosNormal> reflected_mesh_tri_verts; 
  std::vector<VBOPos> shadow_polygon_quad_verts;
  std::vector<VBOPos> light_vert;
  std::vector<VBOPosNormal> mirror_quad_verts;
  std::vector<VBOPosNormal> floor_quad_verts;
  std::vector<VBOPosNormal> reflected_floor_quad_verts;
  std::vector<VBOPos> silhouette_edge_verts;
};

Vec3f ComputeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3);

// ======================================================================
// ======================================================================

#endif




