#ifndef UFO_HPP
#define UFO_HPP

#include "physics.hpp"
#include "vector.hpp"
#include "world.hpp"

class ufo {
public:
  ufo();
  ufo(const vector& v);
  ufo(const world& w);
  
  vector position() const { return _position; };
  void set_position(const vector& v);
  
  void draw() const;
  void erase() const;
  
  static const physics& properties();
private:
  vector _position;
};

#endif
