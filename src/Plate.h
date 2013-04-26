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

  void setVelocity(const Vec3f& vel);
  Plate getOverlap(const Plate& other) const;
  float getArea() const;
  float getNearbyArea() const;
  float getLeft() const;
  float getRight() const;
  float getTop() const;
  float getBottom() const;
  void move();
  void applyForce(const Plate& other);
  void makeEmpty();
  void printPlate() const;
  bool pointInPlate(const Vec3f& pos) const;
  bool pointNearPlate(const Vec3f& pos) const;
  Vec3f getMidpoint() const;
};