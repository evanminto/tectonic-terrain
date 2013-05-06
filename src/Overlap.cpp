/*
 *  Plate.cpp
 *  tectonic-terrain
 *
 *  Created by Evan Minto on 4/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "Overlap.h"

Overlap::Overlap() {
  empty = false;
  center = Vec3f(0,0,0);
  width = 0.0;
}

Overlap::~Overlap() {
  center = Vec3f(0,0,0);
  width = 0.0;
  empty = false;
}

Vec3f Overlap::getMidpoint(const Vec3f& pos) const
{
  float shortestDistance = 1000000;
  int closestFaultPointIdx;

  Vec3f faultPoint1, faultPoint2;

  // Find the closest point to the input position
  for (int i=0; i<faultPoints.size(); i++) {
    float distance = fabs(pos.z() - faultPoints[i].z());

    if (distance < shortestDistance) {
      shortestDistance = distance;
      closestFaultPointIdx = i;
    }
  }

  // If the closest point is the first one, the next closest is the second
  if (closestFaultPointIdx == 0 && faultPoints.size() >= 2) {
    faultPoint1 = faultPoints[closestFaultPointIdx];
    faultPoint2 = faultPoints[closestFaultPointIdx + 1];
  }

  // If the closest point is the last one, the next closest is the second to last
  else if (closestFaultPointIdx == faultPoints.size()-1 && faultPoints.size() >= 2) {
    faultPoint1 = faultPoints[closestFaultPointIdx];
    faultPoint2 = faultPoints[closestFaultPointIdx - 1];
  }

  // Otherwise, pick between the next and previous points
  else if (faultPoints.size() >= 3) {
    faultPoint1 = faultPoints[closestFaultPointIdx];
    float distanceToNext = fabs(faultPoints[closestFaultPointIdx + 1].z() - faultPoint1.z());
    float distanceToPrev = fabs(faultPoints[closestFaultPointIdx - 1].z() - faultPoint1.z());

    if (distanceToNext < distanceToPrev)
      faultPoint2 = faultPoints[closestFaultPointIdx + 1];
    else
      faultPoint2 = faultPoints[closestFaultPointIdx - 1];
  }

  // Make sure the points are ordered so fp1 is higher and fp2 is lower
  if (faultPoint2.z() > faultPoint1.z()) {
    Vec3f temp = faultPoint1;
    faultPoint1 = faultPoint2;
    faultPoint2 = temp;
  }

  // Compute midpoint based on line intersection
  Vec3f midpoint;

  Vec3f direction = faultPoint2 - faultPoint1;
  direction.Normalize();

  midpoint.setz(pos.z());
  float slope = direction.z() / direction.x();
  float deltaZ = faultPoint2.z() - pos.z();
  midpoint.setx(-deltaZ / slope + faultPoint2.x());

  return midpoint;
}

void Overlap::makeEmpty() {
  empty = true;
}

float Overlap::getArea() const {
  return width * fabs(faultPoints[0].z() - faultPoints[faultPoints.size()-1].z());
}

void Overlap::setWidth(float w) {
  width = w;
}
