#ifndef JEWEL_HPP
#define JEWEL_HPP

#include "object.hpp"

class jewel : public object {
public:
  jewel(const world& w, float av, float vv);
  virtual ~jewel();
  
  virtual collision_action_t collision_action() { return COLLISION_ACTION_EXTRA_LIFE; };
  virtual sound* collision_sound();
  
  static const physics& properties();
};

#endif
