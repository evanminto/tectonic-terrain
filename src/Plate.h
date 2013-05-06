/*
 *  Plate.h
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLATE_H
#define PLATE_H

#include <vector>
#include "vectors.h"

class Overlap;

class Plate {
private:
  std::vector<Vec3f> vertices;
  Vec3f velocity;
  Vec3f acceleration;

protected:
  std::vector<Vec3f> faultPoints;
  
public:
  Plate();
  Plate(const Vec3f& p1, const Vec3f& p2, bool faultsLeft);
  ~Plate();

  Vec3f getVelocity() const { return velocity; }
  std::vector<Vec3f> getVertices(bool includeImaginary) const;

  void addFaultPoint(const Vec3f& point);
  void setVelocity(const Vec3f& vel);
  Overlap getOverlap(const Plate& other) const;
  virtual float getArea() const;
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
  void applyForce(const Plate& other, const Overlap& overlap, double timestep);
  void printPlate() const;
  bool pointInPlate(const Vec3f& pos) const;
  bool pointNearPlate(const Vec3f& pos) const;
};

#endif
