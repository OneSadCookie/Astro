#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "object.hpp"

class missile : public object {
public:
  missile(const world& w);
  virtual ~missile();
  
  virtual void step(const vector& ufopos, float dt);
  
  virtual collision_action_t collision_action() { return COLLISION_ACTION_KILL; };
  virtual sound* collision_sound();
  
  static const physics& properties();
};

#endif
