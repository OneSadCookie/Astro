#ifndef TEXTURE_H
#define TEXTURE_H

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

GLuint texture_load_rgba(const char* name);

#if defined(__cplusplus)
}
#endif

#endif
