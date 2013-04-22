/*
 *  Simulation.cpp
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Simulation.h"

Simulation::Simulation() {
  
}

void Simulation::updateOverlap() {
  overlap = plates[0].getOverlap(plates[1]);
}

void Simulation::sortPlates() {
  if (plates.size() >= 2) {
    if (plates[0].getLeft() > plates[1].getLeft() || plates[0].getTop() < plates[1].getTop()) {
      Plate temp = plates[0];
      plates[0] = plates[1];
      plates[1] = temp;
    }
  }
}

void Simulation::addPlate(const Vec3f& p1, const Vec3f& p2) {
  plates.push_back(Plate(p1, p2));
  sortPlates();
  numPlates++;
}

void Simulation::setVelocity(const Vec3f& v1, const Vec3f& v2) {
  plates[0].setVelocity(v1);
  plates[1].setVelocity(v2);
}

void Simulation::setVelocity(const Vec3f& v1) {
  setVelocity(v1, Vec3f(0,0,0));
}

void Simulation::run() {
  for (int i=0; i<numPlates; i++) {
    
    plates[i].move();
    
    for (int j=0; j<numPlates; j++) {
      if (i==j) continue;
      plates[i].applyForce(plates[j]);
    }
  }
  
  updateOverlap();
}

float Simulation::getDisplacement(const Vec3f* pos) const {
  if (overlap.getArea() > 0)
    return overlap.getArea();
  
  return 0;
}

void Simulation::printSimulation() const {
  for (int i=0; i<numPlates; i++) {
    plates[i].printPlate();
  }
  
  std::cout<<"OVERLAP: "<<overlap.getArea()<<std::endl;
  overlap.printPlate();
  std::cout<<std::endl<<std::endl;
}
