#ifndef SOUND_HPP
#define SOUND_HPP

class sound {
public:
  sound(const char* name);
  
  void play();
private:
  void* impl;
};

#endif
