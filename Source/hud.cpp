#include "hud.hpp"

#include "globals.hpp"
#include "main.hpp"
#include "ufo.hpp"

void hud_draw(int lives, unsigned long score, const world& w) {
  int i;
  float y = 2 * ufo::properties().averageRadius;
  unsigned char score_string[128];
  ufo life;
  float scale;
  
  glColor4f(1, 1, 1, 0.5);
  for (i = 0; i < lives; ++i) {
    float x = w.width - (2.5 * i + 2) * ufo::properties().averageRadius;
    life.set_position(vector(x, y, w.z_plane));
    life.draw();
  }
  
  glPushMatrix();
  
  sprintf((char*)score_string, "%lu", score);
  
  glColor4fv(TEXT_COLOR);
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  
  glTranslatef(ufo::properties().averageRadius, 2 * ufo::properties().averageRadius, 0);
  scale = 2 * ufo::properties().averageRadius / GLUT_STROKE_FONT_HEIGHT_EXT;
  glScalef(scale, -scale, scale);
  
  glutStrokeStringEXT(GLUT_STROKE_ROMAN, score_string);
  
  glPopMatrix();
  
  glEnable(GL_TEXTURE_2D);
  
  glPopMatrix();
  
  glColor3f(1, 1, 1);
}

void hud_erase(int lives, unsigned long score, const world& w) {
  int i;
  float y = 2 * ufo::properties().averageRadius;
  unsigned char score_string[128];
  ufo life;
  float scale;

  for (i = 0; i < lives; ++i) {
    float x = w.width - (2.5 * i + 2) * ufo::properties().averageRadius;
    life.set_position(vector(x, y, w.z_plane));
    life.erase();
  }
  
  glPushMatrix();
  
  sprintf((char*)score_string, "%lu", score);
  
  glColor4fv(BACKGROUND_COLOR);
  glDisable(GL_TEXTURE_2D);
  
  glPushMatrix();
  
  glTranslatef(ufo::properties().averageRadius, 2 * ufo::properties().averageRadius, 0);
  scale = 2 * ufo::properties().averageRadius / GLUT_STROKE_FONT_HEIGHT_EXT;
  glScalef(scale, -scale, scale);
  
  glutStrokeStringEXT(GLUT_STROKE_ROMAN, score_string);
  
  glPopMatrix();
  
  glEnable(GL_TEXTURE_2D);
  
  glPopMatrix();
  
  glColor3f(1, 1, 1);
}
