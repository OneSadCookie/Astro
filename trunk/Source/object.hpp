#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "model.hpp"
#include "physics.hpp"
#include "sound.hpp"
#include "vector.hpp"
#include "world.hpp"

enum collision_action_t {
  COLLISION_ACTION_NONE,
  COLLISION_ACTION_KILL,
  COLLISION_ACTION_EXTRA_LIFE
};

class object {
public:
  object(const model* m, const physics& p, const world& w, float av, float vv);
  virtual ~object();
    
  virtual void step(const vector& ufopos, float dt);
  void draw() const;
  void erase() const;
  
  bool collides_with(const vector& v, float r);
  virtual collision_action_t collision_action() = 0;
  virtual sound* collision_sound() = 0;
  
  bool outside_of(const world& w);
protected:
  float _radius;
  
  vector _position;
  vector _velocity;
  
  vector _axis;
  float  _angular_velocity;
  float  _angle;
  
  const model* _model;
  const physics* _physics;
};

#endif
