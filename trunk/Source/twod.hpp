#ifndef TWOD_HPP
#define TWOD_HPP

#include "main.hpp"

class twod {
public:
  twod();
  ~twod();
private:
  GLboolean _depth_test, _texture_2d;
  GLfloat _color[4];
};

#endif
