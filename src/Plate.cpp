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

void Plate::setVelocity(const Vec3f& vel) {
  velocity = vel;
}

Plate Plate::getOverlap(const Plate& other) const {
  Plate newPlate;
  
  if (vertices[1].x() < other.vertices[0].x() || vertices[0].x() > other.vertices[1].x()) {
    newPlate.makeEmpty();
    newPlate.vertices[0].set(vertices[1].x(), 0, vertices[0].z());
    newPlate.vertices[1].set(other.vertices[0].x(), 0, other.vertices[1].z());
  }
  
  else {
    newPlate.vertices[0].set(other.vertices[0].x(), 0, other.vertices[0].z());
    newPlate.vertices[1].set(vertices[1].x(), 0, vertices[1].z());
  }
  
  return newPlate;
}

float Plate::getArea() const {
  float area = (vertices[0].x() - vertices[1].x()) * (vertices[0].z() - vertices[1].z());
  if (area < 0)
    area *= -1;
  if (empty)
    area *= -1;
  
  return area;
}

float Plate::getNearbyArea() const {
  float area = (getNearbyLeft() - getNearbyRight()) * (getNearbyTop() - getNearbyBottom());
  area = fabs(area);

  if (empty)
    area *= -1;
  
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
  
  vertices[0] += velocity;
  vertices[1] += velocity;
}

void Plate::applyForce(const Plate& other, double timestep) {
  Plate overlap = getOverlap(other);
  Vec3f reverseAcceleration = velocity;
  //reverseAcceleration.Normalize();
  reverseAcceleration.Negate();
  acceleration = Vec3f(0,0,0);
  if (true || !overlap.empty) { // Not physically accurate but whatever
    acceleration = reverseAcceleration * 10 * (timestep / 1000.0) * (overlap.getArea() / getArea());
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

Vec3f Plate::getMidpoint() const
{
  return 0.5 * (vertices[0] + vertices[1]);
}

