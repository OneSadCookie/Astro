#include "twod.hpp"

twod::twod() {
  glGetBooleanv(GL_DEPTH_TEST, &_depth_test);
  glGetBooleanv(GL_TEXTURE_2D, &_texture_2d);
  glGetFloatv(GL_CURRENT_COLOR, _color);
  
  if (_depth_test) {
    glDisable(GL_DEPTH_TEST);
  }
  if (_texture_2d) {
    glDisable(GL_TEXTURE_2D);
  }
}

twod::~twod() {
  if (_depth_test) {
    glEnable(GL_DEPTH_TEST);
  }
  if (_texture_2d) {
    glEnable(GL_TEXTURE_2D);
  }
  glColor4fv(_color);
}
