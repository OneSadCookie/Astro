#ifndef SHARD_HPP
#define SHARD_HPP

#include "object.hpp"

class shard : public object {
public:
  shard(const vector& p, const world& w);
  virtual ~shard();
  
  virtual collision_action_t collision_action() { return COLLISION_ACTION_KILL; };
  virtual sound* collision_sound();
  
  static const physics& properties();
};

#endif
