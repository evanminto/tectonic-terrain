/*
 *  Simulation.h
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "Plate.h"

class Simulation {
private:
  std::vector <Plate> plates;
  unsigned int numPlates;
  Plate overlap;
  
private:
  void updateOverlap();
  void sortPlates();
  
public:
  Simulation();

  std::vector<Vec3f> getAllVertices() const;

  void addPlate(const Vec3f& p1, const Vec3f& p2);
  void setVelocity(const Vec3f& v1, const Vec3f& v2);
  void setVelocity(const Vec3f& v1);
  void update(double timestep);
  float getDisplacement(const Vec3f& pos) const;
  void printSimulation() const;
};
