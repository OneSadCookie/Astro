#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "main.hpp"

class button {
public:
  button(GLuint t, float x, float y, float w, float h);
  
  void draw(bool pushed) const;
  void erase(bool pushed) const;
  
  bool hit_by(float x, float y) const;
private:
  GLuint _texture;
  float _x, _y, _w, _h;
};

#endif
