#include "button.hpp"

#include "globals.hpp"
#include "twod.hpp"

button::button(GLuint t, float x, float y, float w, float h)
  : _texture(t), _x(x), _y(y), _w(w), _h(h) {};

void button::draw(bool pushed) const {
  glBindTexture(GL_TEXTURE_2D, _texture);
  
  glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, pushed ? 0.5 : 0);
    glVertex2f(_x, _y);
    glTexCoord2f(1, pushed ? 0.5 : 0);
    glVertex2f(_x + _w, _y);
    glTexCoord2f(1, pushed ? 1.0 : 0.5);
    glVertex2f(_x + _w, _y + _h);
    glTexCoord2f(0, pushed ? 1.0 : 0.5);
    glVertex2f(_x, _y + _h);
  glEnd();
}

void button::erase(bool pushed) const {
  twod save;
  
  glColor4fv(BACKGROUND_COLOR);
  glBegin(GL_TRIANGLE_FAN);
    glVertex2f(_x, _y);
    glVertex2f(_x + _w, _y);
    glVertex2f(_x + _w, _y + _h);
    glVertex2f(_x, _y + _h);
  glEnd();
}

bool button::hit_by(float x, float y) const {
  return x >= _x && y >= _y && x <= _x + _w && y <= _y + _h;
}
