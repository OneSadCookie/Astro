#ifndef GAME_HPP
#define GAME_HPP

#include <list>

using std::list;

#include "object.hpp"
#include "ufo.hpp"

enum direction_t {
  DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT
};

enum game_state_t {
  GAME_STATE_PLAYING,
  GAME_STATE_DYING,
  GAME_STATE_BEING_REBORN,
  GAME_STATE_AWW_MAN_GAME_OVER
};

class game {
public:
  game(int initial_player_lives);
  ~game();
  
  void step(float dt);
  void draw();
  void erase();
  
  void toggle_paused();
  
  void begin_player_move(direction_t d);
  void end_player_move(direction_t d);
  
  game_state_t state() { return _state; };
  unsigned long score() { return _score; };
private:
  game_state_t _state;
  bool         _paused;
  bool         _player_moving[4];
  
  list<object*> _objects;
  ufo*          _player;
  list<object*> _shards;
  
  int           _lives;
  unsigned long _score;
  
  float         _rebirth_progress;
  
  float         _asteroid_velocity;
  
  float         _num_asteroids;
  float         _num_jewels;
  
  float         _asteroid_spawn;
  float         _jewel_spawn;
};

#endif
