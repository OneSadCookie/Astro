#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <map>
#include <string>

using std::map;
using std::string;

#include "main.hpp"
#include "model.hpp"
#include "sound.hpp"
#include "world.hpp"

extern const world WORLD;

extern const float TEXT_COLOR[4];
extern const float BACKGROUND_COLOR[4];

void load_resources();

enum {
  MODEL_ASTEROID,
  MODEL_JEWEL,
  MODEL_MISSILE,
  MODEL_SHARD,
  MODEL_UFO
};

extern const model* MODELS[5];


extern map<string, GLuint> TEXTURES;

enum {
  SOUND_CRASH,
  SOUND_EXTRA_LIFE,
  SOUND_STARTUP
};

extern sound* SOUNDS[3];

#endif
