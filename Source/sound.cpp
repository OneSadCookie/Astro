#include "sound.hpp"
#include "_sound.h"

sound::sound(const char* name) {
  impl = _load_sound(name);
}
  
void sound::play(){
  _play_sound(impl);
}
