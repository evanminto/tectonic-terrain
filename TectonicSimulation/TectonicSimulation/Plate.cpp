/*
 *  Plate.cpp
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "Plate.h"

Plate::Plate() {
  vertices.push_back(Vec3f(0,0,0));
  vertices.push_back(Vec3f(0,0,0));
  empty = false;
}

Plate::Plate(const Vec3f& p1, const Vec3f& p2) {
  vertices.push_back(p1);
  vertices.push_back(p2);
  empty = false;
}

void Plate::setVelocity(const Vec3f& vel) {
  velocity = vel;
}

Plate Plate::getOverlap(const Plate& other) const {
  Plate newPlate;
  
  if (vertices[1].x() < other.vertices[0].x() || vertices[0].x() > other.vertices[1].x()) {
    newPlate.makeEmpty();
    newPlate.vertices[0].set(vertices[0].x(), vertices[0].y(), 0);
    newPlate.vertices[1].set(other.vertices[1].x(), other.vertices[1].y(), 0);
  }
  
  else {
    newPlate.vertices[0].set(other.vertices[0].x(), other.vertices[0].y(), 0);
    newPlate.vertices[1].set(vertices[1].x(), vertices[1].y(), 0);
  }
  
  return newPlate;
}

float Plate::getArea() const {
  float area = (vertices[0].x() - vertices[1].x()) * (vertices[0].y() - vertices[1].y());
  if (area < 0)
    area *= -1;
  if (empty)
    area *= -1;
  
  return area;
}

float Plate::getLeft() const {
  return vertices[0].x();
}

float Plate::getRight() const {
  return vertices[1].x();
}

float Plate::getTop() const {
  return vertices[0].y();
}

float Plate::getBottom() const {
  return vertices[1].y();
}

void Plate::move() {
  velocity += acceleration;
  
  vertices[0] += velocity;
  vertices[1] += velocity;
}

void Plate::applyForce(const Plate& other) {
  Plate overlap = getOverlap(other);
  Vec3f reverseAcceleration = velocity;
  //reverseAcceleration.Normalize();
  reverseAcceleration.Negate();
  acceleration = Vec3f(0,0,0);
  if (!overlap.empty) {
    acceleration = reverseAcceleration * (overlap.getArea() / getArea());
  }
}

void Plate::makeEmpty() {
  empty = true;
}

void Plate::printPlate() const {
  std::cout<<"\t"<<vertices[0]<<std::endl<<"\t"<<vertices[1];
  if (empty)
    std::cout<<std::endl<<"\tempty";
  std::cout<<std::endl;
}
