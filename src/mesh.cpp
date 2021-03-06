#include "mesh.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "argparser.h"

int Triangle::next_triangle_id = 0;

// =======================================================================
// MESH DESTRUCTOR 
// =======================================================================

Mesh::Mesh(ArgParser *_args) {
  args = _args;

  simulation = new Simulation();

  switch(args->preset) {
    case 3:
      simulation->addPlate(Vec3f(-1.0,0,0.1), Vec3f(0.2,0,-1.1), false);
      simulation->addPlate(Vec3f(0.4,0,0.1), Vec3f(1.6,0,-1.1), true);
      break;
    default:
      simulation->addPlate(Vec3f(-0.8,0,0.1), Vec3f(0.48,0,-1.1), false);
      simulation->addPlate(Vec3f(0.52,0,0.1), Vec3f(1.8,0,-1.1), true);
      break;
  }

  switch(args->preset) {
    case 0: // Move together equally
      simulation->setVelocity(Vec3f(0.1, 0, 0), Vec3f(-0.1, 0, 0));
      break;
    case 1: // Move together, right moves faster
      simulation->setVelocity(Vec3f(0.1, 0, 0), Vec3f(-0.3, 0, 0));
      break;
    case 2: // Move together fast
      simulation->setVelocity(Vec3f(0.5, 0, 0), Vec3f(-0.5, 0, 0));
      break;
    case 3: // Move left fast toward, right slow away
      simulation->setVelocity(Vec3f(0.5, 0, 0), Vec3f(0.15, 0, 0));
      break;
    case 4: // Move apart slowly, equally
      simulation->setVelocity(Vec3f(-0.1, 0, 0), Vec3f(0.1, 0, 0));
      break;
    case 5: // Move apart fast
      simulation->setVelocity(Vec3f(-0.5, 0, 0), Vec3f(0.5, 0, 0));
      break;
    case 6: // Move apart at different speeds
      simulation->setVelocity(Vec3f(-0.6, 0, 0), Vec3f(0.1, 0, 0));
      break;
    case 7: // One fault point
      simulation->setVelocity(Vec3f(0.2, 0, 0), Vec3f(-0.2, 0, 0));
      simulation->addFaultPoint(Vec3f(0.7, 0.0, -0.5));
      break;
    case 8: // Two fault points
      simulation->setVelocity(Vec3f(0.5, 0, 0), Vec3f(-0.5, 0, 0));
      simulation->addFaultPoint(Vec3f(0.6, 0.0, -0.3));
      simulation->addFaultPoint(Vec3f(0.45, 0.0, -0.6));
      break;
    case 9: // Three fault points
      simulation->setVelocity(Vec3f(0.4, 0, 0), Vec3f(-0.4, 0, 0));
      simulation->addFaultPoint(Vec3f(0.7, 0.0, -0.25));
      simulation->addFaultPoint(Vec3f(0.55, 0.0, -0.5));
      simulation->addFaultPoint(Vec3f(0.63, 0.0, -0.75));
      break;
  }
}

Mesh::~Mesh() {
  // delete all the triangles
  std::vector<Triangle*> todo;
  for (triangleshashtype::iterator iter = triangles.begin();
       iter != triangles.end(); iter++) {
    Triangle *t = iter->second;
    todo.push_back(t);
  }
  int num_triangles = todo.size();
  for (int i = 0; i < num_triangles; i++) {
    removeTriangle(todo[i]);
  }
  // delete all the vertices
  int num_vertices = numVertices();
  for (int i = 0; i < num_vertices; i++) {
    delete vertices[i];
  }
  cleanupVBOs();
}

// =======================================================================
// MODIFIERS:   ADD & REMOVE
// =======================================================================

Vertex* Mesh::addVertex(const Vec3f &position) {
  int index = numVertices();
  Vertex *v = new Vertex(index, position);
  vertices.push_back(v);
  if (numVertices() == 1)
    bbox = BoundingBox(position,position);
  else 
    bbox.Extend(position);
  return v;
}


void Mesh::addTriangle(Vertex *a, Vertex *b, Vertex *c) {
  // create the triangle
  Triangle *t = new Triangle();
  // create the edges
  Edge *ea = new Edge(a,b,t);
  Edge *eb = new Edge(b,c,t);
  Edge *ec = new Edge(c,a,t);
  // point the triangle to one of its edges
  t->setEdge(ea);
  // connect the edges to each other
  ea->setNext(eb);
  eb->setNext(ec);
  ec->setNext(ea);
  // verify these edges aren't already in the mesh 
  // (which would be a bug, or a non-manifold mesh)
  assert (edges.find(std::make_pair(a,b)) == edges.end());
  assert (edges.find(std::make_pair(b,c)) == edges.end());
  assert (edges.find(std::make_pair(c,a)) == edges.end());
  // add the edges to the master list
  edges[std::make_pair(a,b)] = ea;
  edges[std::make_pair(b,c)] = eb;
  edges[std::make_pair(c,a)] = ec;
  // connect up with opposite edges (if they exist)
  edgeshashtype::iterator ea_op = edges.find(std::make_pair(b,a)); 
  edgeshashtype::iterator eb_op = edges.find(std::make_pair(c,b)); 
  edgeshashtype::iterator ec_op = edges.find(std::make_pair(a,c)); 
  if (ea_op != edges.end()) { ea_op->second->setOpposite(ea); }
  if (eb_op != edges.end()) { eb_op->second->setOpposite(eb); }
  if (ec_op != edges.end()) { ec_op->second->setOpposite(ec); }
  // add the triangle to the master list
  assert (triangles.find(t->getID()) == triangles.end());
  triangles[t->getID()] = t;
}


void Mesh::removeTriangle(Triangle *t) {
  Edge *ea = t->getEdge();
  Edge *eb = ea->getNext();
  Edge *ec = eb->getNext();
  Vertex *a = ea->getStartVertex();
  Vertex *b = eb->getStartVertex();
  Vertex *c = ec->getStartVertex();
  // remove these elements from master lists
  edges.erase(std::make_pair(a,b)); 
  edges.erase(std::make_pair(b,c)); 
  edges.erase(std::make_pair(c,a)); 
  triangles.erase(t->getID());
  // clean up memory
  delete ea;
  delete eb;
  delete ec;
  delete t;
}


// Helper function for accessing data in the hash table
Edge* Mesh::getMeshEdge(Vertex *a, Vertex *b) const {
  edgeshashtype::const_iterator iter = edges.find(std::make_pair(a,b));
  if (iter == edges.end()) return NULL;
  return iter->second;
}

// =======================================================================
// the load function parses very simple .obj files
// =======================================================================

void Mesh::Load() {
  std::string input_file = args->input_file;
  
  FILE *objfile = fopen(input_file.c_str(),"r");
  if (objfile == NULL) {
    std::cout << "ERROR! CANNOT OPEN '" << input_file << "'\n";
    return;
  }

  char line[200] = "";
  char token[100] = "";
  char atoken[100] = "";
  char btoken[100] = "";
  char ctoken[100] = "";
  float x,y,z;
  int a,b,c,d,e;
  
  int index = 0;
  int vert_count = 0;
  int vert_index = 1;
  
  while (fgets(line, 200, objfile)) {   
    int token_count = sscanf (line, "%s\n",token);
    if (token_count == -1) continue;
    a = b = c = d = e = -1;
    if (!strcmp(token,"usemtl") ||
	!strcmp(token,"g")) {
      vert_index = 1; 
      index++;
    } else if (!strcmp(token,"v")) {
      vert_count++;
      sscanf (line, "%s %f %f %f\n",token,&x,&y,&z);
      addVertex(Vec3f(x,y,z));
    } else if (!strcmp(token,"f")) {
      int num = sscanf (line, "%s %s %s %s\n",token,
			atoken,btoken,ctoken);
      sscanf (atoken,"%d",&a);
      sscanf (btoken,"%d",&b);
      sscanf (ctoken,"%d",&c);
      assert (num == 4);
      a -= vert_index;
      b -= vert_index;
      c -= vert_index;
      assert (a >= 0 && a < numVertices());
      assert (b >= 0 && b < numVertices());
      assert (c >= 0 && c < numVertices());
      addTriangle(getVertex(a),getVertex(b),getVertex(c)); 
    } else if (!strcmp(token,"vt")) {
    } else if (!strcmp(token,"vn")) {
    } else if (token[0] == '#') {
    } else {
      printf ("LINE: '%s'",line);
    }
  }
}

// =======================================================================

// calculate the normal of a triangle
Vec3f ComputeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3) {
  Vec3f v12 = p2;
  v12 -= p1;
  Vec3f v23 = p3;
  v23 -= p2;
  Vec3f normal;
  Vec3f::Cross3(normal,v12,v23);
  normal.Normalize();
  return normal;
}

// compute the gouraud normals of all vertices of the mesh and store at each vertex
void Mesh::ComputeGouraudNormals() {
  int i;
  // clear the normals
  for (i = 0; i < numVertices(); i++) {
    getVertex(i)->clearGouraudNormal();  
  }
  // loop through all the triangles incrementing the normal at each vertex
  for (triangleshashtype::iterator iter = triangles.begin();
       iter != triangles.end(); iter++) {
    Triangle *t = iter->second;
    Vec3f n = ComputeNormal((*t)[0]->getPos(),(*t)[1]->getPos(),(*t)[2]->getPos());
    (*t)[0]->incrGouraudNormal(n);
    (*t)[1]->incrGouraudNormal(n);
    (*t)[2]->incrGouraudNormal(n);
  }
  // normalize the sum at each vertex
  for (i = 0; i < numVertices(); i++) {
    getVertex(i)->normalizeGouraudNormal();
  }
}

// =================================================================

void Mesh::displaceVertices() {
  simulation->update(args->timestep);

  //simulation->printSimulation();

  for (int i = 0; i < numVertices(); i++) {
    float displacement = simulation->getDisplacement(vertices[i]->getPos(), args->timestep);
    getVertex(i)->displace((args->timestep / 1000.0) * displacement);
  }
}
