#include "glCanvas.h"
#include "mesh.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "argparser.h"
#include "bmp.h"

Vec3f floor_color(0.9,0.8,0.7);
Vec3f mesh_color(1,1,1);
Vec3f mirror_color(0.1,0.1,0.2);
Vec3f mirror_tint(0.9,0.9,1.0);

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
  for (triangleshashtype::iterator iter = triangles.begin();
       iter != triangles.end(); iter++) {
    Triangle *t = iter->second;
    Vec3f a = (*t)[0]->getPos();
    Vec3f b = (*t)[1]->getPos();
    Vec3f c = (*t)[2]->getPos();    
    Vec3f na = ComputeNormal(a,b,c);
    Vec3f nb = na;
    Vec3f nc = na;
    mesh_tri_verts.push_back(VBOPosNormal(a,na));
    mesh_tri_verts.push_back(VBOPosNormal(b,nb));
    mesh_tri_verts.push_back(VBOPosNormal(c,nc));
  }
  glBindBuffer(GL_ARRAY_BUFFER,mesh_tri_verts_VBO); 
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(VBOPosNormal) * numTriangles() * 3,
	       &mesh_tri_verts[0],
	       GL_STATIC_DRAW); 
}

void Mesh::TextureInit() {
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, texture);
  BitMapFile *image[1];
  image[0] = getBMPData("../tectonic-terrain/test16.bmp");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image[0]->sizeX, image[0]->sizeY, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
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
  glVertexPointer(3, GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(0));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(12));
  glDrawArrays(GL_TRIANGLES,0,numTriangles()*3);
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
  TextureInit();
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
  if (args->glsl_enabled) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    GLint mapLoc = glGetUniformLocation(GLCanvas::program, "terrainMap");
    glUseProgramObjectARB(GLCanvas::program);
    glUniform1i(mapLoc, 0);
  }
  DrawMesh();
  if (args->glsl_enabled) {
    glUseProgramObjectARB(0);
  }

  // -------------------------
  // Render Light (for debugging)
  glColor3f(1,1,0);
  DrawLight();
    
  HandleGLError(); 
}

// =================================================================
