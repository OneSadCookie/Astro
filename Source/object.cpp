#include "object.hpp"

#include "globals.hpp"
#include "main.hpp"

object::object(const model* m, const physics& p, const world& w, float av, float vv)
    : _angle(0), _model(m), _physics(&p) {
  _radius = randommv(p.averageRadius, p.radiusVariance);
  
  _position = vector(w.width * prandom(), -_radius, w.z_plane);
  _velocity = vector(0, randommv(av, vv), 0);
  
  _axis = (p.rotates ? vector(prandom(), prandom(), prandom()) : p.defaultAxis);
  _axis.normalize();
  
  _angular_velocity = randommv(p.averageAngularVelocity, p.angularVelocityVariance);
}

object::~object() {
}

void object::step(const vector& ufopos, float dt) {
  _position = _position + _velocity.scale(dt);

  _angle += _angular_velocity * dt;
  while (_angle < 0) {
    _angle += TWO_PI;
  }
  while (_angle > TWO_PI) {
    _angle -= TWO_PI;
  }
}
  
bool object::outside_of(const world& w) {
  return _position.y - _radius > w.height ||
         _position.y + _radius < 0 ||
         _position.x - _radius > w.width ||
         _position.x + _radius < 0;
}  

void object::draw() const {
  glPushMatrix();
  
  glTranslatef(_position.x, _position.y, _position.z);
  
  float c = cos(_angle);
  float s = sin(_angle);
  
  float matrix[16];

  matrix[0]  = _axis.x * _axis.x * (1 - c) +           c;
  matrix[4]  = _axis.x * _axis.y * (1 - c) - _axis.z * s;
  matrix[8]  = _axis.x * _axis.z * (1 - c) + _axis.y * s;
  matrix[12] = 0;
  
  matrix[1]  = _axis.x * _axis.y * (1 - c) + _axis.z * s;
  matrix[5]  = _axis.y * _axis.y * (1 - c) +           c;
  matrix[9]  = _axis.y * _axis.z * (1 - c) - _axis.x * s;
  matrix[13] = 0;
  
  matrix[2]  = _axis.x * _axis.z * (1 - c) - _axis.y * s;
  matrix[6]  = _axis.y * _axis.z * (1 - c) + _axis.x * s;
  matrix[10] = _axis.z * _axis.z * (1 - c) +           c;
  matrix[14] = 0;
  
  matrix[3]  = 0;
  matrix[7]  = 0;
  matrix[11] = 0;
  matrix[15] = 1;
  
  glMultMatrixf(matrix);
  
  float scale = _radius / _physics->averageRadius;
  glScalef(scale, scale, scale);
  
  _model->draw();
  
  glPopMatrix();
}

void object::erase() const {
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
    glVertex3f(_position.x - _radius, _position.y - _radius, _position.z);
    glVertex3f(_position.x + _radius, _position.y - _radius, _position.z);
    glVertex3f(_position.x + _radius, _position.y + _radius, _position.z);
    glVertex3f(_position.x - _radius, _position.y + _radius, _position.z);
  glEnd();
  glColor4fv(color);
  
  if (texture_2d) {
    glEnable(GL_TEXTURE_2D);
  }
  if (depth_test) {
    glEnable(GL_DEPTH_TEST);
  }
}

bool object::collides_with(const vector& v, float r) {
  return _position.distance(v) < r + _radius;
}
