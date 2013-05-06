/*
 *  Plate.cpp
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <vector>
#include <typeinfo>

#include "Plate.h"
#include "Overlap.cpp"

Plate::Plate() {
  velocity = Vec3f(0,0,0);
  acceleration = Vec3f(0,0,0);
  vertices.push_back(Vec3f(0,0,0));
  vertices.push_back(Vec3f(0,0,0));
}

Plate::Plate(const Vec3f& p1, const Vec3f& p2, bool faultsLeft) {
  velocity = Vec3f(0,0,0);
  acceleration = Vec3f(0,0,0);
  vertices.push_back(p1);
  vertices.push_back(p2);

  if (faultsLeft) {
    addFaultPoint(Vec3f(p1.x(), p1.y(), p1.z()));
    addFaultPoint(Vec3f(p1.x(), p1.y(), p2.z()));
  }
  else {
    addFaultPoint(Vec3f(p2.x(), p1.y(), p1.z()));
    addFaultPoint(Vec3f(p2.x(), p1.y(), p2.z()));
  }
}

std::vector<Vec3f> Plate::getVertices(bool includeImaginary) const {
  std::vector<Vec3f> result;
  if (includeImaginary) {
    result.push_back(vertices[0]);
    result.push_back(Vec3f(vertices[1].x(), 0, vertices[0].z()));
    result.push_back(vertices[1]);
    result.push_back(Vec3f(vertices[0].x(), 0, vertices[1].z()));
  }
  else {
    result.push_back(vertices[0]);
    result.push_back(vertices[1]);
  }
  return result;
}

Plate::~Plate() {
  vertices.clear();
  velocity = Vec3f(0,0,0);
  acceleration = Vec3f(0,0,0);
}

void Plate::addFaultPoint(const Vec3f& point) {
  Vec3f pt = point;
  pt.sety(0.0);

  if (faultPoints.size() >= 2)
    faultPoints.insert(faultPoints.end() - 1, pt);
  else
    faultPoints.push_back(pt);
}

void Plate::setVelocity(const Vec3f& vel) {
  velocity = vel;
}

Overlap Plate::getOverlap(const Plate& other) const {
  Overlap overlap;
  overlap.setWidth(fabs(vertices[1].x() - other.vertices[0].x()));
  
  // If the plates aren't touching, make the overlap empty
  if (vertices[1].x() < other.vertices[0].x() || vertices[0].x() > other.vertices[1].x())
    overlap.makeEmpty();

  // Create the overlap's fault points based on this plate's fault points
  for (int i=0; i<faultPoints.size(); i++) {
    overlap.addFaultPoint(0.5 * (faultPoints[i] + other.faultPoints[i]));
  }

  return overlap;
}

float Plate::getArea() const {
  float area = (vertices[0].x() - vertices[1].x()) * (vertices[0].z() - vertices[1].z());
  if (area < 0)
    area *= -1;
  
  return area;
}

float Plate::getNearbyArea() const {
  float area = (getNearbyLeft() - getNearbyRight()) * (getNearbyTop() - getNearbyBottom());
  area = fabs(area);
  
  return area;
}

float Plate::getWidth() const {
  return fabs(getLeft() - getRight());
}

float Plate::getHeight() const {
  return fabs(getTop() - getBottom());
}

float Plate::getNearbyWidth() const {
  return fabs(getNearbyLeft() - getNearbyRight());
}

float Plate::getNearbyHeight() const {
  return fabs(getNearbyTop() - getNearbyBottom());
}

float Plate::getLeft() const {
  return vertices[0].x();
}

float Plate::getRight() const {
  return vertices[1].x();
}

float Plate::getTop() const {
  return vertices[0].z();
}

float Plate::getBottom() const {
  return vertices[1].z();
}

float Plate::getNearbyLeft() const {
  return getLeft() - getWidth() * 0.2;
}

float Plate::getNearbyRight() const {
  return getRight() + getWidth() * 0.2;
}

float Plate::getNearbyTop() const {
  return getTop() + getHeight() * 0.2;
}

float Plate::getNearbyBottom() const {
  return getBottom() - getHeight() *0.2;
}

void Plate::update(double timestep) {
  velocity += (timestep / 1000.0) * acceleration;
  
  vertices[0] += (timestep / 1000.0) * velocity;
  vertices[1] += (timestep / 1000.0) * velocity;

  for (int i=0; i<faultPoints.size(); i++) {
    faultPoints[i] += (timestep / 1000.0) * velocity;
  }
}

void Plate::applyForce(const Plate& other, const Overlap& overlap, double timestep) {
  Vec3f reverseVelocity = velocity;
  reverseVelocity.Negate();

  // Create friction force based on reversed velocity and the ratio of overlap area to plate area.
  // This ensures the force only stops the velocity and doesn't reverse it.
  acceleration = reverseVelocity * (sqrt(fabs(5 * overlap.getArea())) / sqrt(getArea()));
  
  // Just in case, make sure the force doesn't reverse the velocity
  if (acceleration.Length() > velocity.Length())
    acceleration = reverseVelocity;
}

void Plate::printPlate() const {
  std::cout<<"\t"<<vertices[0]<<std::endl<<"\t"<<vertices[1];
  std::cout<<std::endl;
}

bool Plate::pointInPlate(const Vec3f& pos) const {
  if (pos.x() > vertices[0].x() && pos.x() < vertices[1].x() && pos.z() > vertices[1].z() && pos.z() < vertices[0].z())
    return true;
  return false;
}

bool Plate::pointNearPlate(const Vec3f& pos) const {
  if (pos.x() > vertices[0].x() - 0.2 && pos.x() < vertices[1].x() + 0.2 && pos.z() > vertices[1].z() - 0.1 && pos.z() < vertices[0].z() + 0.1)
    return true;
  return false;
}

