#include "message.hpp"

#include "twod.hpp"

message::message(const char* message, const char* explanation, const world& wrld) {
  strncpy((char*)_message, message, MAX_MESSAGE_LENGTH);
  _message[MAX_MESSAGE_LENGTH] = '\0';
  strncpy((char*)_explanation, explanation, MAX_EXPLANATION_LENGTH);
  _explanation[MAX_EXPLANATION_LENGTH] = '\0';
  
  _my = WORLD.height / 2;
  _sm = (WORLD.height / 6) / GLUT_STROKE_FONT_HEIGHT_EXT;
  float w = glutStrokeLength(GLUT_STROKE_ROMAN, _message);
  _mx = (wrld.width - _sm * w) / 2;
  
  _ey = 2 * WORLD.height / 3;
  _se = (WORLD.height / 12) / GLUT_STROKE_FONT_HEIGHT_EXT;
  w = glutStrokeLength(GLUT_STROKE_ROMAN, _explanation);
  _ex = (wrld.width - _se * w) / 2;
}
  
void message::draw() {
  twod save;

  glColor4fv(TEXT_COLOR);

  glPushMatrix();
  glTranslatef(_mx, _my, 0);
  glScalef(_sm, -_sm, _sm);
  glutStrokeStringEXT(GLUT_STROKE_ROMAN, _message);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(_ex, _ey, 0);
  glScalef(_se, -_se, _se);
  glutStrokeStringEXT(GLUT_STROKE_ROMAN, _explanation);
  glPopMatrix();
}

void message::erase() {
  twod save;

  glColor4fv(BACKGROUND_COLOR);

  glPushMatrix();
  glTranslatef(_mx, _my, 0);
  glScalef(_sm, -_sm, _sm);
  glutStrokeStringEXT(GLUT_STROKE_ROMAN, _message);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(_ex, _ey, 0);
  glScalef(_se, -_se, _se);
  glutStrokeStringEXT(GLUT_STROKE_ROMAN, _explanation);
  glPopMatrix();
}
