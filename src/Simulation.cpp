/*
 *  Simulation.cpp
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Simulation.h"
#include "perlin.cpp"


float randomFloat(float a, float b) {
  float random = ((float) rand()) / (float) RAND_MAX;
  float diff = b - a;
  float r = random * diff;
  return a + r;
}

Simulation::Simulation() {
  
}

std::vector<Vec3f> Simulation::getAllVertices() const {
  std::vector<Vec3f> result;
  for (int i=0; i<plates.size(); i++) {
    std::vector<Vec3f> plateVerts = plates[i].getVertices(true);
    result.insert(result.end(), plateVerts.begin(), plateVerts.end());
  }
  return result;
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

void Simulation::update(double timestep) {
  for (unsigned int i=0; i<numPlates; i++) {
    
    plates[i].update(timestep);
    
    for (unsigned int j=0; j<numPlates; j++) {
      if (i==j) continue;
      plates[i].applyForce(plates[j], timestep);
    }
  }
  
  updateOverlap();
}

float Simulation::getDisplacement(const Vec3f& pos) const {

  //float area = overlap.getArea();
  float amplitude = 0.3 * (sqrt(plates[0].getVelocity().Length()) + sqrt(plates[1].getVelocity().Length()));//0.001;
  if (overlap.isEmpty())
    amplitude *= -1;
  Vec3f center = overlap.getMidpoint();
  float spread = 0.5 * overlap.getNearbyWidth();

  float y = amplitude * exp(-1 * (pos.x() - center.x()) * (pos.x() - center.x()) / (2 * spread * spread));

  float displacement = y;

  // Area factor
  //displacement *= sqrt(area);

  // Velocity factor
  //displacement *= (1 + sqrt(plates[0].getVelocity().Length()) + sqrt(plates[1].getVelocity().Length()));

  // Random factor
  displacement += -0.3 * y * perlinNoise(randomFloat(0, 1), randomFloat(0, 1));

  std::cout<<displacement<<std::endl;
  
  return displacement;
}

void Simulation::printSimulation() const {
  for (unsigned int i=0; i<numPlates; i++) {
    plates[i].printPlate();
  }
  
  std::cout<<"OVERLAP: "<<overlap.getArea()<<std::endl;
  overlap.printPlate();
  std::cout<<std::endl<<std::endl;
}
