#pragma once

#include <raylib.h>

#ifndef BREAKOUT_H
#define BREAKOUT_H

#define GRAVITY 10

#define PADDLE_SIZE ((Vector2){.x = 200, .y = 30})
#define PADDLE_SPEED 300

#define BALL_MAX_SPEED ((Vector2){.x = 800, .y = 800})
#define BALL_SIZE ((Vector2){.x = 20, .y = 20})
#define BALL_DEFAULT_POS ((Vector2){.x = 400 + PADDLE_SIZE.x / 2, .y = 400})

#define BLOCK_SIZE ((Vector2){.x = 90, .y = 25})
#define BLOCK_SPACING 40

#define HIT_SCORE 20
#define CAMERA_SHAKE 2.f
#define CAMERA_RECOVER 0.5

typedef struct {
  Vector2 pos;
  bool is_alive;
} block_t;

typedef struct {
  Vector2 pos;
  Vector2 vel;
  float particle_timer;
} ball_t;

typedef struct {
  Vector2 pos;
  Vector2 vel;
} paddle_t;

typedef struct {
  Vector2 pos;
  Vector2 vel;
  Vector2 size;
  float original_lifetime;
  float lifetime;
  bool active;
} particle_t;

typedef enum {
  WIN,
  LOSE,
  PAUSED,
  RUNNING,
} state_t;

typedef struct {
  paddle_t player;
  ball_t ball;

  block_t blocks[36];
  particle_t particles[1024];

  int lives;
  state_t state;
  unsigned int score;
  Camera2D camera;
  float shakeness;

} game_t;

typedef struct {
  Sound hit;
  Sound title_screen;
} assets_t;

void breakout_init();
void breakout_update();
void breakout_render();
void breakout_deinit();

#endif
