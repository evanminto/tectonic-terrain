/*
 *  Simulation.cpp
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Simulation.h"
#define PI 3.14


float randomFloat(float a, float b) {
  float random = ((float) rand()) / (float) RAND_MAX;
  float diff = b - a;
  float r = random * diff;
  return a + r;
}

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

void Simulation::update() {
  for (int i=0; i<numPlates; i++) {
    
    plates[i].move();
    
    for (int j=0; j<numPlates; j++) {
      if (i==j) continue;
      plates[i].applyForce(plates[j]);
    }
  }
  
  updateOverlap();
}

float Simulation::getDisplacement(const Vec3f& pos) const {
  if (!overlap.pointNearPlate(pos))
    return 0;

  bool inOverlap = false;
  if (overlap.pointInPlate(pos))
    inOverlap = true;

  float area = overlap.getArea();
  float nearbyArea = overlap.getNearbyArea();
  float distanceFromMid = pos.x() - overlap.getMidpoint().x();
  //float displacement = 0.00001 * area / (distanceFromMid * distanceFromMid);

  float a = area;
  float amplitude = 0.01;
  Vec3f center = overlap.getMidpoint();
  float spread = 0.15;

  float y = amplitude * exp(-1 * (pos.x() - center.x()) * (pos.x() - center.x())/ (2 * spread * spread));

  float displacement = y;

  // Area factor
  displacement *= area;

  // Velocity factor
  displacement *= (1 + plates[0].getVelocity().Length() + plates[1].getVelocity().Length());

  // Noise
  displacement += (randomFloat(-0.008, 0.008));

  //std::cout<<abs(pos.x() - overlap.getMidpoint().x())<<std::endl;
  
  return displacement;
}

void Simulation::printSimulation() const {
  for (int i=0; i<numPlates; i++) {
    plates[i].printPlate();
  }
  
  std::cout<<"OVERLAP: "<<overlap.getArea()<<std::endl;
  overlap.printPlate();
  std::cout<<std::endl<<std::endl;
}
