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
#include "Overlap.h"

class Simulation {
private:
  std::vector <Plate> plates;
  unsigned int numPlates;
  Overlap overlap;
  
private:
  void updateOverlap();
  void sortPlates();
  
public:
  Simulation();
  ~Simulation();

  std::vector<Vec3f> getAllVertices() const;

  void addPlate(const Vec3f& p1, const Vec3f& p2, bool faultsLeft);
  void setVelocity(const Vec3f& v1, const Vec3f& v2);
  void setVelocity(const Vec3f& v1);
  void update(double timestep);
  float getDisplacement(const Vec3f& pos, double timestep) const;
  void printSimulation() const;
  void addFaultPoint(const Vec3f& point);
};
