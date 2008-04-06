#include "game.hpp"

#include "globals.hpp"
#include "main.hpp"

#include "asteroid.hpp"
#include "jewel.hpp"
#include "shard.hpp"

#include "hud.hpp"
#include "message.hpp"

static const float UFO_VELOCITY = 15;
static const float VV_FRACTION = 0.1;
static const float INITIAL_ASTEROID_VELOCITY = 10;

static const float ASTEROID_VELOCITY_INCREASE = 0.038888;
static const float ASTEROID_NUMBER_INCREASE = 0.016666;
static const float JEWEL_NUMBER_INCREASE = 0.000088;

static const float INITIAL_NUM_ASTEROIDS = 10;
static const float INITIAL_NUM_JEWELS = 0.04;

static message *PAUSED_MESSAGE;

static const float REBIRTH_TIME = 1;

game::game(int ipl) 
    : _state(GAME_STATE_BEING_REBORN), _paused(false), _lives(ipl),
      _score(0), _rebirth_progress(0), _asteroid_velocity(INITIAL_ASTEROID_VELOCITY),
      _num_asteroids(INITIAL_NUM_ASTEROIDS), _num_jewels(INITIAL_NUM_JEWELS),
      _asteroid_spawn(0), _jewel_spawn(0) {
  _player_moving[0] = false;
  _player_moving[1] = false;
  _player_moving[2] = false;
  _player_moving[3] = false;
  
  _player = new ufo(WORLD);
}

game::~game() {
  delete _player;
  for (list<object*>::iterator i = _objects.begin(); i != _objects.end(); ++i) {
    delete (*i);
  }
  for (list<object*>::iterator i = _shards.begin(); i != _shards.end(); ++i) {
    delete (*i);
  }
}
  
void game::step(float dt) {
  if (_paused) {
    return;
  }
  
  _asteroid_velocity += dt * ASTEROID_VELOCITY_INCREASE;
  _num_asteroids += dt * ASTEROID_NUMBER_INCREASE;
  _num_jewels += dt * JEWEL_NUMBER_INCREASE;
  
  if (_state != GAME_STATE_BEING_REBORN) {
    _jewel_spawn += dt;
    if (_jewel_spawn > (WORLD.height / _asteroid_velocity) / _num_jewels) {
      _objects.push_back(new jewel(WORLD,
                                   _asteroid_velocity,
                                   VV_FRACTION * _asteroid_velocity));
      _jewel_spawn -= (WORLD.height / _asteroid_velocity) / _num_jewels;
    }
    
    _asteroid_spawn += dt;
    if (_asteroid_spawn > (WORLD.height / _asteroid_velocity) / _num_asteroids) {
      _objects.push_back(new asteroid(WORLD,
                                      _asteroid_velocity,
                                      VV_FRACTION * _asteroid_velocity));
      _asteroid_spawn -= (WORLD.height / _asteroid_velocity) / _num_asteroids;
    }
  } else {
    _rebirth_progress += dt;
    if (_rebirth_progress > REBIRTH_TIME) {
      _state = GAME_STATE_PLAYING;
      _rebirth_progress = 0;
    }
  }
    
  for (list<object*>::iterator i = _objects.begin(); i != _objects.end(); ++i) {
    (*i)->step(_player->position(), dt);
  }
  
  if (_state == GAME_STATE_DYING) {
    bool all_shards_gone = true;
    for (list<object*>::iterator i = _shards.begin(); i != _shards.end(); ++i) {
      if (!(*i)->outside_of(WORLD)) {
        all_shards_gone = false;
      }
      (*i)->step(_player->position(), dt);
    }
    if (all_shards_gone) {
      for (list<object*>::iterator i = _shards.begin(); i != _shards.end(); ++i) {
      	delete (*i);
      }
      _shards.clear();
      _state = (_lives < 0 ? GAME_STATE_AWW_MAN_GAME_OVER : GAME_STATE_BEING_REBORN);
      
      for (list<object*>::iterator i = _objects.begin(); i != _objects.end(); ++i) {
      	delete (*i);
      }
      _objects.clear();
      
      delete _player;
      _player = new ufo(WORLD);
    }
  }
  
  if (_state == GAME_STATE_PLAYING) {
    vector ufo_position = _player->position();
    if (_player_moving[size_t(DIRECTION_RIGHT)]) {
      ufo_position.x += UFO_VELOCITY * dt;
    }
    if (_player_moving[size_t(DIRECTION_LEFT)]) {
      ufo_position.x -= UFO_VELOCITY * dt;
    }
    if (_player_moving[size_t(DIRECTION_DOWN)]) {
      ufo_position.y += UFO_VELOCITY * dt;
    }
    if (_player_moving[size_t(DIRECTION_UP)]) {
      ufo_position.y -= UFO_VELOCITY * dt;
    }
    if (ufo_position.x < ufo::properties().averageRadius) {
      ufo_position.x = ufo::properties().averageRadius;
    }
    if (ufo_position.x > WORLD.width - ufo::properties().averageRadius) {
      ufo_position.x = WORLD.width - ufo::properties().averageRadius;
    }
    if (ufo_position.y < ufo::properties().averageRadius) {
      ufo_position.y = ufo::properties().averageRadius;
    }
    if (ufo_position.y > WORLD.height - ufo::properties().averageRadius) {
      ufo_position.y = WORLD.height - ufo::properties().averageRadius;
    }
    _player->set_position(ufo_position);
  }
  
  list<object*>::iterator j = _objects.begin();
  for (list<object*>::iterator i = _objects.begin(); i != _objects.end(); ++i) {
    bool gone = false;
  
    ++j;
    if (_state != GAME_STATE_DYING &&
        (*i)->collides_with(_player->position(), ufo::properties().averageRadius)) {
      gone = true;
      switch ((*i)->collision_action()) {
      case COLLISION_ACTION_NONE: {
      } break;
      case COLLISION_ACTION_KILL: {
        --_lives;
        _state = GAME_STATE_DYING;
        
        for (int k = 0; k < 50; ++k) {
          _shards.push_back(new shard(_player->position(), WORLD));
        }
      } break;
      case COLLISION_ACTION_EXTRA_LIFE: {
        ++_lives;
      } break;
      default: {
        assert(false);
      };
      }
      
      (*i)->collision_sound()->play();
    }
    
    if ((*i)->outside_of(WORLD)) {
      if (_state == GAME_STATE_PLAYING) {
        ++_score;
      }
      gone = true;
    }
      
    if (gone) {
      delete (*i);
      _objects.erase(i);
      
      i = j;
      --i;
    }
  }
}

void game::draw() {
  for (list<object*>::iterator i = _objects.begin(); i != _objects.end(); ++i) {
    (*i)->draw();
  }
  if (_state == GAME_STATE_DYING) {
    for (list<object*>::iterator i = _shards.begin(); i != _shards.end(); ++i) {
      (*i)->draw();
    }
  } else {
    if (_state == GAME_STATE_BEING_REBORN) {
      float a = _rebirth_progress/REBIRTH_TIME;
      glColor4f(1, 1, 1, a);
    }
    _player->draw();
    glColor4f(1, 1, 1, 1);
  }
  
  hud_draw(_lives, _score, WORLD);
  if (_paused) {
    PAUSED_MESSAGE->draw();
  }
}

void game::erase() {
  for (list<object*>::iterator i = _objects.begin(); i != _objects.end(); ++i) {
    (*i)->erase();
  }
  if (_state == GAME_STATE_DYING) {
    for (list<object*>::iterator i = _shards.begin(); i != _shards.end(); ++i) {
      (*i)->erase();
    }
  } else {
    _player->erase();
  }
  
  hud_erase(_lives, _score, WORLD);
  if (_paused) {
    PAUSED_MESSAGE->erase();
  }
}
  
void game::toggle_paused() {
  if (!PAUSED_MESSAGE) {
    PAUSED_MESSAGE =
      new message("Paused", "Press space to continue", WORLD);
  }
  
  _paused = !_paused;
}
  
void game::begin_player_move(direction_t d) {
  _player_moving[size_t(d)] = true;
}

void game::end_player_move(direction_t d) {
  _player_moving[size_t(d)] = false;
}
