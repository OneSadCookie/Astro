#include "main.hpp"

#include "button.hpp"
#include "game.hpp"
#include "globals.hpp"
#include "message.hpp"
#include "preferences.h"

static const int INITIAL_PLAYER_LIVES = 3;
static const int FRAMES_PER_SECOND = 200;
static const int FRAME_DELAY = (int)(double(1000)/double(FRAMES_PER_SECOND));

static const int MAX_NAME_LENGTH = 32;

static unsigned long _frameNumber = 0;
static unsigned long _stepNumber = 0;
static int _prevStepTime = -1;

static game* _game = NULL;

static const char* GAME_NAME = "Astro";

static message* _title_message = NULL;
static char _hs_name[MAX_NAME_LENGTH];
static unsigned long _highscore;
static bool _hs_enter_mode = false;

static const button* PLAY_BUTTON = NULL;
static const button* QUIT_BUTTON = NULL;

static bool _play_pushed = false;
static bool _quit_pushed = false;
static bool _play_tracking = false;
static bool _quit_tracking = false;

void display();
void reshape(int w, int h);
void idle();

void key_down(unsigned char k, int x, int y);
void key_up(unsigned char k, int x, int y);
void special_down(int k, int x, int y);
void special_up(int k, int x, int y);

void mouse(int button, int state, int x, int y);
void mouse_motion(int x, int y);

void cleanup();

void new_game();
void make_message();

void unproject(int x, int y, float& ox, float& oy);

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(400, 300);
#if defined(USE_GLUT_GAME_MODE)
  glutGameModeString("640x480:32");
  if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
    (void)glutEnterGameMode();
    glutSetCursor(GLUT_CURSOR_NONE);
  } else {
    (void)glutCreateWindow("Astro");
  }
#else
  (void)glutCreateWindow("Astro");
#endif
  glutDisplayFunc(display);
	glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  
  glutKeyboardFunc(key_down);
  glutKeyboardUpFunc(key_up);
  glutSpecialFunc(special_down);
  glutSpecialUpFunc(special_up);
  
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  
  atexit(cleanup);
  
  srandom(time(NULL));
  
  load_resources();
  
  strcpy(_hs_name, "Anonymous");
  getStringPreference("name", _hs_name, MAX_NAME_LENGTH);
  _highscore = getIntegerPreference("highscore", 0);
  make_message();
  /*
  FRAMES_PER_SECOND = getIntegerPreference("physicsrate", 100);
  printf("Physics running at %d fps\n", FRAMES_PER_SECOND);
  */
  float qw = WORLD.width / 4;
  PLAY_BUTTON = new button(TEXTURES["play"], 3 * qw, WORLD.height - qw / 2, qw, qw / 2);
  QUIT_BUTTON = new button(TEXTURES["quit"], 0, WORLD.height - qw / 2, qw, qw / 2);
  
  SOUNDS[SOUND_STARTUP]->play();
  
  glutMainLoop();

	return 0;
}

void display() {
  if (_game != NULL) {

    int now = glutGet(GLUT_ELAPSED_TIME);
    
    int stepExtra = (now - _prevStepTime) % FRAME_DELAY;
    int steps = (now - _prevStepTime) / FRAME_DELAY;
    
    if (_prevStepTime != -1) {
      for (int i = 0; i < steps; ++i) {
        _game->step(1.0f/FRAMES_PER_SECOND);
      }
    }
  
    glClear(GL_DEPTH_BUFFER_BIT);
  
    _game->draw();
    
    glutSwapBuffers();
    
    _game->erase();
    
    ++_frameNumber;
    
    if (_prevStepTime != -1) {
      _stepNumber += steps;
    }
    _prevStepTime = now - stepExtra;
  
  } else {
    glClear(GL_DEPTH_BUFFER_BIT);
  
    _title_message->draw();
    PLAY_BUTTON->draw(_play_pushed);
    QUIT_BUTTON->draw(_quit_pushed);
    
    glutSwapBuffers();
    
    PLAY_BUTTON->erase(_play_pushed);
    QUIT_BUTTON->erase(_quit_pushed);
    _title_message->erase();
  }
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_TEXTURE_2D);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  float l, r, b, t;
  float window_aspect = float(w) / float(h);
  float game_aspect = WORLD.width / WORLD.height;
  float k;
    
  if (window_aspect > game_aspect) {
    k = WORLD.height / float(h);
    t = 0;
    b = WORLD.height;
    l = (k * w - WORLD.width) * -0.5f;
    r = l + k * w;
  } else {
    k = WORLD.width / float(w);
    l = 0;
    r = WORLD.width;
    t = (k * h - WORLD.height) * -0.5f;
    b = t + k * h;
  }
  
  glOrtho(l, r, b, t, -4, 4);
  glMatrixMode(GL_MODELVIEW);
  
  glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1],
               BACKGROUND_COLOR[2], BACKGROUND_COLOR[3]);
  glClear(GL_COLOR_BUFFER_BIT);
}

void idle() {
  if (_game != NULL) {
    if (_game->state() == GAME_STATE_AWW_MAN_GAME_OVER) {
      if (_game->score() > _highscore) {
        _highscore = _game->score();
        putIntegerPreference("highscore", _highscore);
        _hs_name [0] = '\0';
        _hs_enter_mode = true;
        make_message();
      }
    
      delete _game;
      _game = NULL;
    }
  }

  glutPostRedisplay();
}

void cleanup() {
  writePreferencesToDisk();
}

void key_down(unsigned char k, int x, int y) {
  if (_hs_enter_mode) {
    int i = strlen(_hs_name);
    
    if (k == '\n' || k == '\r') {
      _hs_enter_mode = false;
      putStringPreference("name", _hs_name);
    }
    
    _hs_name[i] = (char)k;
    _hs_name[i + 1] = '\0';
    
    make_message();
    glutPostRedisplay();
    
    if (i + 1 == MAX_NAME_LENGTH - 1) {
      _hs_enter_mode = false;
      putStringPreference("name", _hs_name);
    }
  } else {
    switch(k) {
      case ' ': {
        if (_game != NULL) {
          _game->toggle_paused();
        }
      } break;
      case 'q': case 'Q': {
        exit(EXIT_SUCCESS);
      } break;
      case 'p': case 'P': {
        if (_game == NULL) {
          new_game();
        }
      } break;
      default: break;
    }
  }
}

void key_up(unsigned char k, int x, int y) {
}

void special_down(int k, int x, int y) {
  if (_game != NULL) {
    switch(k) {
    case GLUT_KEY_UP: _game->begin_player_move(DIRECTION_UP); break;
    case GLUT_KEY_DOWN: _game->begin_player_move(DIRECTION_DOWN); break;
    case GLUT_KEY_LEFT: _game->begin_player_move(DIRECTION_LEFT); break;
    case GLUT_KEY_RIGHT: _game->begin_player_move(DIRECTION_RIGHT); break;
    default: break;
    }
  }
}

void special_up(int k, int x, int y) {
  if (_game != NULL) {
    switch(k) {
    case GLUT_KEY_UP: _game->end_player_move(DIRECTION_UP); break;
    case GLUT_KEY_DOWN: _game->end_player_move(DIRECTION_DOWN); break;
    case GLUT_KEY_LEFT: _game->end_player_move(DIRECTION_LEFT); break;
    case GLUT_KEY_RIGHT: _game->end_player_move(DIRECTION_RIGHT); break;
    default: break;
    }
  }
}

void mouse(int button, int state, int x, int y) {
  if (button != GLUT_LEFT_BUTTON || _game != NULL) {
    return;
  }
  
  float ox, oy;
  unproject(x, y, ox, oy);
  
  if (state == GLUT_UP) {
    if (PLAY_BUTTON->hit_by(ox, oy) && _play_tracking) {
      _play_pushed = false;
      _play_tracking = false;
      new_game();
    } else if (QUIT_BUTTON->hit_by(ox, oy) && _quit_tracking) {
      _play_pushed = false;
      _play_tracking = false;
      exit(EXIT_SUCCESS);
    }
  } else {
    if (PLAY_BUTTON->hit_by(ox, oy)) {
      _play_pushed = true;
      _play_tracking = true;
    } else if (QUIT_BUTTON->hit_by(ox, oy)) {
      _quit_pushed = true;
      _quit_tracking = true;
    }
    glutPostRedisplay();
  }
}

void mouse_motion(int x, int y) {
  if (_game != NULL) {
    return;
  }

  float ox, oy;
  unproject(x, y, ox, oy);
  
  if (PLAY_BUTTON->hit_by(ox, oy)) {
    _play_pushed = _play_tracking;
  } else {
    _play_pushed = false;
  }
  if (QUIT_BUTTON->hit_by(ox, oy)) {
    _quit_pushed = _quit_tracking;
  } else {
    _quit_pushed = false;
  }
  
  //glutPostRedisplay();
}

void new_game() {
  delete _game;
  _game = new game(INITIAL_PLAYER_LIVES);
  _frameNumber = 0;
	_stepNumber = 0;
	_prevStepTime = -1;
}

void make_message() {
  delete _title_message;
  
  char buffer[64];
  sprintf(buffer, "%s%s: %lu", _hs_name, (_hs_enter_mode ? "|" : ""), _highscore);
  
  _title_message = new message(GAME_NAME, buffer, WORLD);
}

void unproject(int x, int y, float& ox, float& oy) {
  GLdouble model[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  GLdouble proj[16];
  glGetDoublev(GL_PROJECTION_MATRIX, proj);
  GLint view[4];
  glGetIntegerv(GL_VIEWPORT, view);
  
  GLdouble _ox, _oy, _oz;
  
  gluUnProject (x, view[3] - y, 0, model, proj, view, &_ox, &_oy, &_oz);
  ox = _ox;
  oy = _oy;
}
