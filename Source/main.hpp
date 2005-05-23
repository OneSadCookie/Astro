#ifndef MAIN_H
#define MAIN_H

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "glutext.h"

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

inline float prandom() {
  return float((double)random() / (double)INT_MAX);
}

inline float randommv(float mean, float variance) {
  return mean + (2 * variance * prandom() - variance);
}

const float PI = 3.141592654;
const float TWO_PI = 2 * PI;

#endif
