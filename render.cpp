#include "glCanvas.h"
#include "mesh.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "argparser.h"

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
  tmp += Vec3f(cos(args->timer) * (max.x()-min.x()), 0, 0);
  tmp += Vec3f(0,0,sin(args->timer) * (max.z()-min.z()));
  return tmp;
}

void Mesh::initializeVBOs() {
  glGenBuffers(1,&mesh_tri_verts_VBO);
  //glGenBuffers(1,&reflected_mesh_tri_verts_VBO);
  //glGenBuffers(1,&shadow_polygon_quad_verts_VBO);
  glGenBuffers(1,&light_vert_VBO);
  //glGenBuffers(1,&mirror_quad_verts_VBO);
  //glGenBuffers(1,&floor_quad_verts_VBO);
  //glGenBuffers(1,&reflected_floor_quad_verts_VBO);
  //glGenBuffers(1,&silhouette_edge_verts_VBO);
  bbox.initializeVBOs();
}

void Mesh::cleanupVBOs() {
  glDeleteBuffers(1,&mesh_tri_verts_VBO);
  //glDeleteBuffers(1,&reflected_mesh_tri_verts_VBO);
  //glDeleteBuffers(1,&shadow_polygon_quad_verts_VBO);
  glDeleteBuffers(1,&light_vert_VBO);
  //glDeleteBuffers(1,&mirror_quad_verts_VBO);
  //glDeleteBuffers(1,&floor_quad_verts_VBO);
  //glDeleteBuffers(1,&reflected_floor_quad_verts_VBO);
  //glDeleteBuffers(1,&silhouette_edge_verts_VBO);
  bbox.cleanupVBOs();
}

// ================================================================================
// ================================================================================

void Mesh::SetupLight(Vec3f light_position) {
  light_vert.push_back(VBOPos(light_position));
  glBindBuffer(GL_ARRAY_BUFFER,light_vert_VBO); 
  glBufferData(GL_ARRAY_BUFFER,sizeof(VBOPos)*1,&light_vert[0],GL_STATIC_DRAW); 
}

void Mesh::SetupMirror() {
/*  Vec3f diff = bbox.getMax()-bbox.getMin();
  // create frame vertices just a bit bigger than the bounding box
  Vec3f a = bbox.getMin() + Vec3f(-0.25*diff.x(), 0.1*diff.y(),-0.25*diff.z());
  Vec3f b = bbox.getMin() + Vec3f(-0.25*diff.x(), 0.1*diff.y(), 1.25*diff.z());
  Vec3f c = bbox.getMin() + Vec3f(-0.25*diff.x(), 1.25*diff.y(),-0.25*diff.z());
  Vec3f d = bbox.getMin() + Vec3f(-0.25*diff.x(), 1.25*diff.y(), 1.25*diff.z());
  Vec3f normal = ComputeNormal(a,c,b);
  mirror_quad_verts.push_back(VBOPosNormal(a,normal));
  mirror_quad_verts.push_back(VBOPosNormal(c,normal));
  mirror_quad_verts.push_back(VBOPosNormal(d,normal));
  mirror_quad_verts.push_back(VBOPosNormal(b,normal));
  glBindBuffer(GL_ARRAY_BUFFER,mirror_quad_verts_VBO); 
  glBufferData(GL_ARRAY_BUFFER,sizeof(VBOPosNormal)*4,&mirror_quad_verts[0],GL_STATIC_DRAW); */
}

void Mesh::SetupFloor() {
 /* Vec3f diff = bbox.getMax()-bbox.getMin();
  // create vertices just a bit bigger than the bounding box
  Vec3f a = bbox.getMin() + Vec3f(-0.75*diff.x(),0,-0.75*diff.z());
  Vec3f b = bbox.getMin() + Vec3f( 1.75*diff.x(),0,-0.75*diff.z());
  Vec3f c = bbox.getMin() + Vec3f(-0.75*diff.x(),0, 1.75*diff.z());
  Vec3f d = bbox.getMin() + Vec3f( 1.75*diff.x(),0, 1.75*diff.z());
  Vec3f normal = ComputeNormal(a,c,d);
  floor_quad_verts.push_back(VBOPosNormal(a,normal));
  floor_quad_verts.push_back(VBOPosNormal(c,normal));
  floor_quad_verts.push_back(VBOPosNormal(d,normal));
  floor_quad_verts.push_back(VBOPosNormal(b,normal));
  glBindBuffer(GL_ARRAY_BUFFER,floor_quad_verts_VBO); 
  glBufferData(GL_ARRAY_BUFFER,sizeof(VBOPosNormal)*4,&floor_quad_verts[0],GL_STATIC_DRAW); */
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
    if (args->gouraud_normals) {
      na = (*t)[0]->getGouraudNormal();
      nb = (*t)[1]->getGouraudNormal();
      nc = (*t)[2]->getGouraudNormal();
    }
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


// draw a second copy of the object where it appears to be on the other side of the mirror
void Mesh::SetupReflectedMesh() {

  // ASSIGNMENT: WRITE THIS FUNCTION

}


// draw a second copy of the floor where it appears to be on the other side of the mirror
void Mesh::SetupReflectedFloor() {

  // ASSIGNMENT: WRITE THIS FUNCTION

}


// figure out which edges are the silhouette of the object 
void Mesh::SetupSilhouetteEdges(Vec3f light_position) {

  // ASSIGNMENT: WRITE THIS FUNCTION

}


// project the silhouette edges away from the light source
void Mesh::SetupShadowPolygons(Vec3f light_position) {

  // ASSIGNMENT: WRITE THIS FUNCTION

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

void Mesh::DrawMirror() {
/*  glBindBuffer(GL_ARRAY_BUFFER, mirror_quad_verts_VBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(0));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(12));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mirror_quad_verts_VBO);
  glDrawArrays(GL_QUADS,0,4);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);*/
}

void Mesh::DrawFloor() {
 /* glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.1,4.0);
  glBindBuffer(GL_ARRAY_BUFFER, floor_quad_verts_VBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(0));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(12));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_quad_verts_VBO);
  glDrawArrays(GL_QUADS,0,4);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_POLYGON_OFFSET_FILL);*/
}

void Mesh::DrawMesh() {
    assert ((int)mesh_tri_verts.size() == numTriangles()*3);
  if (args->silhouette_edges) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.1,4.0);
  }
  glBindBuffer(GL_ARRAY_BUFFER, mesh_tri_verts_VBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(0));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(12));
  glDrawArrays(GL_TRIANGLES,0,numTriangles()*3);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  if (args->silhouette_edges) {
    glDisable(GL_POLYGON_OFFSET_FILL);
  }
}

void Mesh::DrawReflectedMesh() {
  if (reflected_mesh_tri_verts.size() > 0) {
    assert ((int)reflected_mesh_tri_verts.size() == numTriangles()*3);
    glBindBuffer(GL_ARRAY_BUFFER, reflected_mesh_tri_verts_VBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(0));
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(12));
    glDrawArrays(GL_TRIANGLES,0,numTriangles()*3);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

void Mesh::DrawReflectedFloor() {
  if (reflected_floor_quad_verts.size() > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, reflected_floor_quad_verts_VBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(0));
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(VBOPosNormal), BUFFER_OFFSET(12));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reflected_floor_quad_verts_VBO);
    glDrawArrays(GL_QUADS,0,4);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

void Mesh::DrawSilhouetteEdges() {
  if (silhouette_edge_verts.size() > 0) {
    glLineWidth(4);
    glDisable(GL_LIGHTING);
    glBindBuffer(GL_ARRAY_BUFFER, silhouette_edge_verts_VBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VBOPos), BUFFER_OFFSET(0));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, silhouette_edge_verts_VBO);
    glDrawArrays(GL_LINES,0,silhouette_edge_verts.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
  }
}

void Mesh::DrawShadowPolygons() {
  if (shadow_polygon_quad_verts.size() > 0) {
    glDisable(GL_LIGHTING);
    glBindBuffer(GL_ARRAY_BUFFER, shadow_polygon_quad_verts_VBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VBOPos), BUFFER_OFFSET(0));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadow_polygon_quad_verts_VBO);
    glDrawArrays(GL_QUADS,0,shadow_polygon_quad_verts.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
  }
}

// ======================================================================================
// ======================================================================================

void Mesh::setupVBOs() {
  // delete all the old geometry
  mesh_tri_verts.clear(); 
  reflected_mesh_tri_verts.clear(); 
  shadow_polygon_quad_verts.clear();
  light_vert.clear();
  mirror_quad_verts.clear();
  floor_quad_verts.clear();
  reflected_floor_quad_verts.clear();
  silhouette_edge_verts.clear();
  cleanupVBOs();
  // setup the new geometry
  Vec3f light_position = LightPosition();
  SetupLight(light_position);
  //SetupMirror();
  //SetupFloor();
  SetupMesh();
  SetupReflectedMesh();
  //SetupReflectedFloor();
  SetupSilhouetteEdges(light_position);
  SetupShadowPolygons(light_position);
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
  // NEITHER SHADOWS NOR MIRROR
  if (!args->mirror && !args->shadow) {
    InsertColor(mirror_color);
    //DrawMirror();
    InsertColor(floor_color);
    //DrawFloor();
    if (args->geometry) {
      InsertColor(mesh_color);
      if (args->glsl_enabled) {
          glUseProgramObjectARB(GLCanvas::program);
      }
      DrawMesh();
      if (args->glsl_enabled) {
	glUseProgramObjectARB(0);
      }
    }
  } 

  // ---------------------
  // MIRROR ONLY RENDERING
  else if (args->mirror && !args->shadow) {
    // Clear frame, depth & stencil buffers
    glClearColor(1,1,1,0);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Draw all non-mirror geometry to frame & depth buffers
    InsertColor(mesh_color);
    DrawMesh();
    InsertColor(floor_color);
    //DrawFloor();
    // draw back of mirror (if we can see the back)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    InsertColor(mirror_color);
    //DrawMirror();     
    glDisable(GL_CULL_FACE);

    // Draw mirror to stencil buffer, where depth buffer passes
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glStencilFunc(GL_ALWAYS,1,~0); 
    // (only draw the mirror if we can see the front)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    InsertColor(mirror_color);
    //DrawMirror();     
    glDisable(GL_CULL_FACE);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE); // disable frame buffer writes      
    glDepthRange(1,1);
    glDepthFunc(GL_ALWAYS);
    glStencilFunc(GL_EQUAL,1,~0); 
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
    //DrawMirror();     

    // Set depth to infinity, where stencil buffer passes
    glDepthFunc(GL_LESS);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); // enable frame buffer writes
    glDepthRange(0,1);

    // Draw reflected geometry to frame & depth buffer, where stencil buffer passes
    InsertColor(mirror_tint * mesh_color);
    DrawReflectedMesh();
    InsertColor(mirror_tint * floor_color);
    DrawReflectedFloor();

    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE); // disable frame buffer writes      
    glStencilOp(GL_KEEP,GL_KEEP,GL_ZERO);
    glDepthFunc(GL_ALWAYS);
    //DrawMirror();
    glDepthFunc(GL_LESS);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); // enable frame buffer writes    

    glDisable(GL_STENCIL_TEST);
  } 

  // ---------------------
  // SHADOW ONLY RENDERING
  else if (!args->mirror && args->shadow) {
    
  // ASSIGNMENT: WRITE THIS RENDERING MODE

  }

  // --------------------------
  // MIRROR & SHADOW!
  else {
    assert (args->mirror && args->shadow);

    // EXTRA CREDIT: IMPLEMENT THIS INTERACTION

    glColor3f(0.3,0.3,0.3);
    //DrawFloor();
    DrawMesh();
    //DrawMirror();
  }

  // -------------------------
  // ADDITIONAL VISUALIZATIONS (for debugging)
  glColor3f(1,1,0);
  DrawLight();
  if (args->reflected_geometry) {
    glColor3f(0,0,1);
    DrawReflectedMesh();
    DrawReflectedFloor();
  }
  if (args->bounding_box) {
    glColor3f(0,0,0);
    bbox.drawVBOs();
  }
  if (args->silhouette_edges) {
    glColor3f(1,0,0);
    DrawSilhouetteEdges();
  }
  if (args->shadow_polygons) {
    glDisable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glColor4f(0,1,0.5,0.2);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    DrawShadowPolygons();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_LIGHTING);
  }
    
  HandleGLError(); 
}

// =================================================================
