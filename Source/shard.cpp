#include "globals.hpp"

#include "shard.hpp"
#include "ufo.hpp"

static const float SHARD_AV = 40;
static const float SHARD_VV = 10;

static const physics SHARD_PHYSICS = {
  0.4,			//averageRadius
  0.2,    	//radiusVariance
  
  true,		  //rotates
  vector(),	//defaultAxis (unused)
  
  2 * PI,		//averageAngularVelocity;
  PI 			  //angularVelocityVariance;
};

shard::shard(const vector& p, const world& w)
    : object(MODELS[MODEL_SHARD], SHARD_PHYSICS, w, SHARD_AV, SHARD_VV) {
  vector t;
  t.x = 2 * prandom() - 1;
  t.y = 2 * prandom() - 1;
  t.z = 0;
  t.normalize();
  _position = t.scale(ufo::properties().averageRadius) + p;
  _velocity = t.scale(randommv(SHARD_AV, SHARD_VV));
}

shard::~shard() {
}

sound* shard::collision_sound() {
  return SOUNDS[SOUND_CRASH];
}

const physics& shard::properties() {
  return SHARD_PHYSICS;
}
