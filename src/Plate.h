/*
 *  Plate.h
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "vectors.h"

class Plate {
private:
  std::vector <Vec3f> vertices;
  Vec3f velocity;
  Vec3f acceleration;
  bool empty;
  
public:
  Plate();
  Plate(const Vec3f& p1, const Vec3f& p2);

  Vec3f getVelocity() const { return velocity; }
  bool isEmpty() const { return empty; }
  std::vector<Vec3f> getVertices(bool includeImaginary) const;

  void setVelocity(const Vec3f& vel);
  Plate getOverlap(const Plate& other) const;
  float getArea() const;
  float getNearbyArea() const;
  float getWidth() const;
  float getHeight() const;
  float getNearbyWidth() const;
  float getNearbyHeight() const;
  float getLeft() const;
  float getRight() const;
  float getTop() const;
  float getBottom() const;
  float getNearbyLeft() const;
  float getNearbyRight() const;
  float getNearbyTop() const;
  float getNearbyBottom() const;
  void update(double timestep);
  void applyForce(const Plate& other, double timestep);
  void makeEmpty();
  void printPlate() const;
  bool pointInPlate(const Vec3f& pos) const;
  bool pointNearPlate(const Vec3f& pos) const;
  Vec3f getMidpoint() const;
};
