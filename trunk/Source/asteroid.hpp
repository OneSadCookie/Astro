#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "object.hpp"

class asteroid : public object {
public:
  asteroid(const world& w, float av, float vv);
  virtual ~asteroid();
  
  virtual collision_action_t collision_action() { return COLLISION_ACTION_KILL; };
  virtual sound* collision_sound();
  
  static const physics& properties();
};

#endif
