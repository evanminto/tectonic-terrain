/*
 *  Plate.h
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OVERLAP_H
#define OVERLAP_H

#include <vector>
#include "vectors.h"
#include "Plate.h"

class Plate;

class Overlap : public Plate {
private:
  Vec3f center;
  float width;
  bool empty;
  
public:
  Overlap();
  bool isEmpty() const { return empty; }
  Vec3f getMidpoint(const Vec3f& pos) const;
  void makeEmpty();
  float getArea() const;
  float getWidth() const { return width; }
  void setWidth(float w);
};

#endif
