#include "globals.hpp"

#include "jewel.hpp"

static const physics JEWEL_PHYSICS = {
  0.5,			//averageRadius
  0.1,    	//radiusVariance
    
  true,		  //rotates
  vector(),	//defaultAxis (unused)
  
  2 * PI,		//averageAngularVelocity;
  PI 			  //angularVelocityVariance;
};

jewel::jewel(const world& w, float av, float vv)
  : object(MODELS[MODEL_JEWEL], JEWEL_PHYSICS, w, av, vv) {
}

jewel::~jewel() {
}

sound* jewel::collision_sound() {
  return SOUNDS[SOUND_EXTRA_LIFE];
}

const physics& jewel::properties() {
  return JEWEL_PHYSICS;
}
