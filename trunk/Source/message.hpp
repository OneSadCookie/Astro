#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "globals.hpp"

const size_t MAX_MESSAGE_LENGTH = 32;
const size_t MAX_EXPLANATION_LENGTH = 128;

class message {
public:
  message(const char* message, const char* explanation, const world& w);
  
  void draw();
  void erase();
private:
  unsigned char _message[MAX_MESSAGE_LENGTH + 1];
  unsigned char _explanation[MAX_EXPLANATION_LENGTH + 1];
  
  float _sm, _se;
  float _mx, _my, _ex, _ey;
};

#endif
