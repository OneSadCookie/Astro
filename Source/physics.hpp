#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "vector.hpp"

struct physics {
  float  averageRadius;
  float  radiusVariance;
  
  bool   rotates;
  vector defaultAxis; // unused if rotates is true
  
  float  averageAngularVelocity;
  float  angularVelocityVariance;
};

#endif
