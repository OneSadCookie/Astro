#include "globals.hpp"

#include "asteroid.hpp"

static const physics ASTEROID_PHYSICS = {
  1.0,			//averageRadius
  0.1,    	//radiusVariance
    
  true,		  //rotates
  vector(),	//defaultAxis (unused)
  
  2 * PI,		//averageAngularVelocity;
  PI 			  //angularVelocityVariance;
};

asteroid::asteroid(const world& w, float av, float vv)
  : object(MODELS[MODEL_ASTEROID], ASTEROID_PHYSICS, w, av, vv) {
}

asteroid::~asteroid() {
}

sound* asteroid::collision_sound() {
  return SOUNDS[SOUND_CRASH];
}

const physics& asteroid::properties() {
  return ASTEROID_PHYSICS;
}
