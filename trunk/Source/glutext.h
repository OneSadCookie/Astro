#ifndef GLUT_EXT_H
#define GLUT_EXT_H

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*
    This header defines commands that should (IMHO) be in GLUT, but aren't.
 */

#ifdef __cplusplus
extern "C" {
#endif



#define GLUT_STROKE_FONT_ASCENT_EXT  119.05
#define GLUT_STROKE_FONT_DESCENT_EXT 33.33
#define GLUT_STROKE_FONT_HEIGHT_EXT  (GLUT_STROKE_FONT_ASCENT_EXT + GLUT_STROKE_FONT_DESCENT_EXT)

void glutStrokeStringEXT(void *font, const unsigned char* string);



#ifdef __cplusplus
}
#endif

#endif
