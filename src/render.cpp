#include "glCanvas.h"
#include "mesh.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "argparser.h"
#include "bmp.h"

Vec3f mesh_color(1.0,1.0,1.0);

// Array of texture indices
static unsigned int texture[5];

// =======================================================================
// =======================================================================

void InsertColor(const Vec3f &v) {
  glColor3f(v.x(),v.y(),v.z());
}

// the light position can be animated
Vec3f Mesh::LightPosition() {
  Vec3f min = bbox.getMin();
  Vec3f max = bbox.getMax();
  Vec3f tmp;
  bbox.getCenter(tmp);
  tmp += Vec3f(0,1.5*(max.y()-min.y()),0);
  tmp += Vec3f(cos(0) * (max.x()-min.x()), 0, 0);
  tmp += Vec3f(0,2,sin(0) * (max.z()-min.z()));
  return tmp;
}

void Mesh::initializeVBOs() {
  glGenBuffers(1,&mesh_tri_verts_VBO);
  glGenBuffers(1,&light_vert_VBO);
  bbox.initializeVBOs();
  TextureInit();
}

void Mesh::cleanupVBOs() {
  glDeleteBuffers(1,&mesh_tri_verts_VBO);
  glDeleteBuffers(1,&light_vert_VBO);
  bbox.cleanupVBOs();
}

// ================================================================================
// ================================================================================

void Mesh::SetupLight(Vec3f light_position) {
  light_vert.push_back(VBOPos(light_position));
  glBindBuffer(GL_ARRAY_BUFFER,light_vert_VBO); 
  glBufferData(GL_ARRAY_BUFFER,sizeof(VBOPos)*1,&light_vert[0],GL_STATIC_DRAW); 
}

void Mesh::SetupMesh() {
  ComputeGouraudNormals();
  for (triangleshashtype::iterator iter = triangles.begin();
       iter != triangles.end(); iter++) {
    Triangle *t = iter->second;
    Vec3f a = (*t)[0]->getPos();
    Vec3f b = (*t)[1]->getPos();
    Vec3f c = (*t)[2]->getPos();    
    Vec3f na = (*t)[0]->getGouraudNormal();
    Vec3f nb = (*t)[1]->getGouraudNormal();
    Vec3f nc = (*t)[2]->getGouraudNormal();
    Vec3f color = mesh_color;
    mesh_tri_verts.push_back(VBOPosNormalColorTexture(a,na,color,a.x()*1,a.z()*-1));
    mesh_tri_verts.push_back(VBOPosNormalColorTexture(b,nb,color,b.x()*1,b.z()*-1));
    mesh_tri_verts.push_back(VBOPosNormalColorTexture(c,nc,color,c.x()*1,c.z()*-1));
  }
  glBindBuffer(GL_ARRAY_BUFFER,mesh_tri_verts_VBO); 
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(VBOPosNormalColorTexture) * numTriangles() * 3,
	       &mesh_tri_verts[0],
	       GL_STATIC_DRAW); 
}

void Mesh::TextureInit() {
  glEnable(GL_TEXTURE_2D);

  // Generate and load textures
  glGenTextures(5, texture);
  BitMapFile *image[5];
  image[0] = getBMPData(args->heightmap_filename);
  image[1] = getBMPData("normal.bmp");
  image[2] = getBMPData("stone.bmp");
  image[3] = getBMPData("grass.bmp");
  image[4] = getBMPData("snow.bmp");

  // Set Texture 0 - Heightmap
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[0]->sizeX, image[0]->sizeY, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

  // Set Texture 1 - Normal map
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[1]->sizeX, image[1]->sizeY, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);
  
  // Set Texture 2 - Stone
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[2]->sizeX, image[2]->sizeY, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);

  // Set Texture 3 - Grass
  glBindTexture(GL_TEXTURE_2D, texture[3]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[3]->sizeX, image[3]->sizeY, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, image[3]->data);

  // Set Texture 4 - Snow
  glBindTexture(GL_TEXTURE_2D, texture[4]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[4]->sizeX, image[4]->sizeY, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, image[4]->data);

  glDisable(GL_TEXTURE_2D);
}

// ================================================================================
// ================================================================================

void Mesh::DrawLight() {
  glPointSize(10);
  glDisable(GL_LIGHTING);
  glBindBuffer(GL_ARRAY_BUFFER, light_vert_VBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBOPos), BUFFER_OFFSET(0));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_vert_VBO);
  glDrawArrays(GL_POINTS,0,1);
  glDisableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_LIGHTING);
}

void Mesh::DrawMesh() {
    assert ((int)mesh_tri_verts.size() == numTriangles()*3);
  glBindBuffer(GL_ARRAY_BUFFER, mesh_tri_verts_VBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBOPosNormalColorTexture), BUFFER_OFFSET(0));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(VBOPosNormalColorTexture), BUFFER_OFFSET(12));
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_FLOAT, sizeof(VBOPosNormalColorTexture), BUFFER_OFFSET(24));

  // Texture
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer( 2, GL_FLOAT, sizeof(VBOPosNormalColorTexture), BUFFER_OFFSET(36));
  
  glDrawArrays(GL_TRIANGLES,0,numTriangles()*3);
  
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_TEXTURE_2D);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

// ======================================================================================
// ======================================================================================

void Mesh::setupVBOs() {
  // delete all the old geometry
  mesh_tri_verts.clear(); 
  light_vert.clear();
  cleanupVBOs();
  // setup the new geometry
  Vec3f light_position = LightPosition();
  SetupLight(light_position);
  SetupMesh();
  bbox.setupVBOs();
}

void Mesh::drawVBOs() {
  // scale it so it fits in the window
  Vec3f center; bbox.getCenter(center);
  float s = 1/bbox.maxDim();
  glScalef(s,s,s);
  glTranslatef(-center.x(),-center.y(),-center.z());

  // setup the light
  Vec3f light_position = LightPosition();
  GLfloat position[4] = { float(light_position.x()),float(light_position.y()),float(light_position.z()),1 };
  glLightfv(GL_LIGHT1, GL_POSITION, position);

  // --------------------------
  // Render Mesh 
  InsertColor(mesh_color);
  glEnable(GL_TEXTURE_2D);
  if (args->glsl_enabled) {
    glUseProgramObjectARB(GLCanvas::program);
    glActiveTexture(GL_TEXTURE0);

    // Height Map
    glActiveTexture(GL_TEXTURE0);
    GLint mapLoc = glGetUniformLocationARB(GLCanvas::program, "terrainMap");
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glUniform1iARB(mapLoc, 0);

    // Normal Map
    glActiveTexture(GL_TEXTURE1);
    GLint normLoc = glGetUniformLocationARB(GLCanvas::program, "normalMap");
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glUniform1iARB(normLoc, 1);

    // Stone Texture
    glActiveTexture(GL_TEXTURE2);
    GLint stoneLoc = glGetUniformLocationARB(GLCanvas::program, "texStone");
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glUniform1iARB(stoneLoc, 2);

    // Grass Texture
    glActiveTexture(GL_TEXTURE3);
    GLint grassLoc = glGetUniformLocationARB(GLCanvas::program, "texGrass");
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glUniform1iARB(grassLoc, 3);

    // Snow Texture
    glActiveTexture(GL_TEXTURE4);
    GLint snowLoc = glGetUniformLocationARB(GLCanvas::program, "texSnow");
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glUniform1iARB(snowLoc, 4);

    glActiveTexture(GL_TEXTURE0);
  }
  else {
    glBindTexture(GL_TEXTURE_2D, texture[0]);
  }
  DrawMesh();
  if (args->glsl_enabled) {
    glUseProgramObjectARB(0);
  }
  glDisable(GL_TEXTURE_2D);
  
  // -------------------------
  // Render Light (for debugging)
  glColor3f(1,1,0);
  DrawLight();
    
  HandleGLError(); 
}

// =================================================================
