#include "globals.hpp"

#include "texture.h"

const world WORLD = {
  40.0,
  30.0,
  0.0
};

const float TEXT_COLOR[4] = { 1, 0, 0, 1 };
const float BACKGROUND_COLOR[4] = { 0.005, 0, 0.1, 1 };

const model* MODELS[5];
map<string, GLuint> TEXTURES;
sound* SOUNDS[3];

void load_resources() {
  MODELS[MODEL_ASTEROID] = new model("asteroid.a3d");
  MODELS[MODEL_JEWEL] = new model("jewel.a3d");
  MODELS[MODEL_MISSILE] = new model("missile.a3d");
  MODELS[MODEL_SHARD] = new model("shard.a3d");
  MODELS[MODEL_UFO] = new model("ufo.a3d");

  SOUNDS[SOUND_CRASH] = new sound("crash.aiff");
  SOUNDS[SOUND_EXTRA_LIFE] = new sound("extra_life.aiff");
  SOUNDS[SOUND_STARTUP] = new sound("startup.wav");
  
  TEXTURES["asteroid"] = texture_load_rgba("asteroid.rgba");
  TEXTURES["jewel"] = texture_load_rgba("jewel.rgba");
  TEXTURES["ufo"] = texture_load_rgba("ufo.rgba");
  TEXTURES["play"] = texture_load_rgba("play.rgba");
  TEXTURES["quit"] = texture_load_rgba("quit.rgba");
}
