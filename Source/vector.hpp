#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>

struct vector {
  float x, y, z;
  
  vector() {};
  vector(float a, float b, float c) : x(a), y(b), z(c) {};
  
  float  dot(const vector& v) const { return x * v.x + y * v.y + z * v.z; };
  vector scale(float f) const { return vector(f * x, f * y, f * z); };
  float  length() const { return sqrt(x * x + y * y + z * z); };
  vector add(const vector& v) const { return vector(x + v.x, y + v.y, z + v.z); };
  vector operator + (const vector& v) const { return add(v); };
  float  distance(const vector& v) const { return add(v.scale(-1)).length(); };
  void   normalize() { float s = 1/length(); x *= s, y *= s; z *= s; };
};

#endif
