#include "globals.hpp"
#include "main.hpp"
#include "ufo.hpp"
#include "model.hpp"

static const physics UFO_PHYSICS = {
  1.0,			//averageRadius
  0.0,			//radiusVariance
  
  false,    //rotates;
  vector(),	//defaultAxis (unused anyway :P)
  
  0.0,			//averageAngularVelocity
  0.0				//angularVelocityVariance
};

ufo::ufo() {}

ufo::ufo(const vector& v)
  : _position(v) {}

ufo::ufo(const world& w) {
  _position = vector(w.width / 2, w.height - UFO_PHYSICS.averageRadius, w.z_plane);
}
  
void ufo::set_position(const vector& v) {
  _position = v;
}

void ufo::draw() const {
  glPushMatrix();
  
  glTranslatef(_position.x, _position.y, _position.z);
  
  MODELS[MODEL_UFO]->draw();
  
  glPopMatrix();
}

void ufo::erase() const {
  float r = UFO_PHYSICS.averageRadius;

  GLboolean depth_test;
  glGetBooleanv(GL_DEPTH_TEST, &depth_test);
  
  GLboolean texture_2d;
  glGetBooleanv(GL_TEXTURE_2D, &texture_2d);
  
  GLfloat color[4];
  glGetFloatv(GL_CURRENT_COLOR, color);
  
  if (depth_test) {
    glDisable(GL_DEPTH_TEST);
  }
  if (texture_2d) {
    glDisable(GL_TEXTURE_2D);
  }

  glColor4fv(BACKGROUND_COLOR);
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(_position.x - r, _position.y - r, _position.z);
    glVertex3f(_position.x + r, _position.y - r, _position.z);
    glVertex3f(_position.x + r, _position.y + r, _position.z);
    glVertex3f(_position.x - r, _position.y + r, _position.z);
  glEnd();
  glColor4fv(color);
  
  if (texture_2d) {
    glEnable(GL_TEXTURE_2D);
  }
  if (depth_test) {
    glEnable(GL_DEPTH_TEST);
  }
}

const physics& ufo::properties() {
  return UFO_PHYSICS;
}
