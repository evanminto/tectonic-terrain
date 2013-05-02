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
}

Vec3f Overlap::getMidpoint(const Vec3f& pos) const
{
  float shortestDistance = 1000000;
  int closestFaultPointIdx;

  Vec3f faultPoint1, faultPoint2;

  for (int i=0; i<faultPoints.size(); i++) {
    float distance = fabs(pos.z() - faultPoints[i].z());

    if (distance < shortestDistance) {
      shortestDistance = distance;
      closestFaultPointIdx = i;
    }
  }

  if (closestFaultPointIdx == 0 && faultPoints.size() >= 2) {
    faultPoint1 = faultPoints[closestFaultPointIdx];
    faultPoint2 = faultPoints[closestFaultPointIdx + 1];
  }

  else if (closestFaultPointIdx == faultPoints.size()-1 && faultPoints.size() >= 2) {
    faultPoint1 = faultPoints[closestFaultPointIdx];
    faultPoint2 = faultPoints[closestFaultPointIdx - 1];
  }

  else if (faultPoints.size() >= 3) {
    faultPoint1 = faultPoints[closestFaultPointIdx];
    float distanceToNext = fabs(faultPoints[closestFaultPointIdx + 1].z() - faultPoint1.z());
    float distanceToLast = fabs(faultPoints[closestFaultPointIdx - 1].z() - faultPoint1.z());

    if (distanceToNext < distanceToLast)
      faultPoint2 = faultPoints[closestFaultPointIdx + 1];
    else
      faultPoint2 = faultPoints[closestFaultPointIdx - 1];
  }

  if (faultPoint2.z() > faultPoint1.z()) {
    Vec3f temp = faultPoint1;
    faultPoint1 = faultPoint2;
    faultPoint2 = temp;
  }

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
