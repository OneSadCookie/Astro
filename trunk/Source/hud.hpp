#ifndef HUD_HPP
#define HUD_HPP

#include "world.hpp"

void hud_draw(int lives, unsigned long score, const world& w);
void hud_erase(int lives, unsigned long score, const world& w);

#endif
