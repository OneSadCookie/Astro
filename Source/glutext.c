#include "glutext.h"

void glutStrokeStringEXT(void *font, const unsigned char* string) {
  const unsigned char* s = string;
  while (*s) {
    glutStrokeCharacter(font, *s);
    ++s;
  } 
}
