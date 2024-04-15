#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>

#include "./breakout.h"
#include "utils.h"

static game_t g;

void breakout_init() {
  memset(&g, 0, sizeof(g));

  g = (game_t){.player = {.pos = {.x = 400, .y = 500}},
               .ball = {.pos = BALL_DEFAULT_POS, .vel = {.x = 0, .y = 0}},
               .state = PAUSED,
               .lives = 5};

  int a = 0;
  for (int i = 0; i < 6; i++) {
    a += 1;
    float x = 20 + i * (BLOCK_SIZE.x + BLOCK_SPACING);
    for (int j = 0; j < 6; j++) {
      a += 1;
      float y = 30 + j * (BLOCK_SIZE.y + BLOCK_SPACING / 2);
      int index = ARR2D(g.blocks, 6, i, j);
      g.blocks[index] = (block_t){.pos =
                                      {
                                          .x = x,
                                          .y = y,
                                      },
                                  .is_alive = true};
    }
  }
}

static void particle_spawn(Vector2 vel, Vector2 pos, float lifetime) {
  for (int i = 0; i < ARR_LEN(g.particles); i++) {
    if (!(g.particles[i].active)) {
      g.particles[i].pos = pos;
      g.particles[i].vel = vel;
      g.particles[i].size = (Vector2){.x = 10, .y = 10};
      g.particles[i].active = true;
      g.particles[i].lifetime = lifetime;
      g.particles[i].original_lifetime = lifetime;
      return;
    }
  }
}

static void particle_update() {
  float delta = GetFrameTime();
  for (int i = 0; i < ARR_LEN(g.particles); i++) {
    if (g.particles[i].active) {
      g.particles[i].lifetime -= delta;
      g.particles[i].pos = Vector2Add(Vector2Scale(g.particles[i].vel, delta),
                                      g.particles[i].pos);
      g.particles[i].vel.y += GRAVITY;
      if (g.particles[i].lifetime < 0.f) {
        g.particles[i].active = false;
      }
    }
  }
}

static void update_player() {
  float delta = GetFrameTime();

  if (IsKeyDown(KEY_LEFT)) {
    g.player.vel.x -= PADDLE_SPEED * delta;
  }

  if (IsKeyDown(KEY_RIGHT)) {
    g.player.vel.x += PADDLE_SPEED * delta;
  }

  g.player.pos.x += g.player.vel.x;
  g.player.vel.x *= 0.8;
  g.player.pos.x = Clamp(g.player.pos.x, 0, 800 - PADDLE_SIZE.x);
}

static void title_screen_template(const char *title) {
  DrawRectangle(0, 0, 800, 600,
                (Color){
                    .r = 0,
                    .g = 0,
                    .b = 0,
                    .a = 127,
                });
  int length = MeasureText(title, 48);
  DrawText(title, (800 / 2) - length / 2, 600 / 2 - 47, 48, BLACK);
  DrawText(title, (800 / 2) - length / 2, 600 / 2 - 48, 48, RED);
  length = MeasureText("PRESS SPACE TO CONTINUE", 32);
  DrawText("PRESS SPACE TO CONTINUE", (800 / 2) - length / 2, 600 / 2 - 32 + 47,
           32, BLACK);
  DrawText("PRESS SPACE TO CONTINUE", (800 / 2) - length / 2, 600 / 2 - 32 + 48,
           32, WHITE);
}

static void update_ball() {
  float delta = GetFrameTime();

  g.ball.pos = Vector2Add(g.ball.pos, Vector2Scale(g.ball.vel, delta));
  g.ball.vel.y += GRAVITY;

  if (g.ball.pos.y > 600) {
    g.ball.pos = BALL_DEFAULT_POS;
    g.ball.vel = Vector2Zero();
    g.lives -= 1;
  }

  Rectangle ball_rect = (Rectangle){
      .x = g.ball.pos.x,
      .y = g.ball.pos.y,
      .width = BALL_SIZE.x,
      .height = BALL_SIZE.y,
  };

  // player
  Rectangle player_rect = (Rectangle){
      .x = g.player.pos.x,
      .y = g.player.pos.y,
      .width = PADDLE_SIZE.x,
      .height = PADDLE_SIZE.y,
  };

  if (CheckCollisionRecs(player_rect, ball_rect) && g.ball.vel.y > 0) {
    g.ball.vel.y = -g.ball.vel.y * 2.5f;
    g.ball.vel.x = g.ball.vel.x + (g.player.vel.x * 12.0f);
  }

  if (g.ball.pos.y < 0 && g.ball.vel.y < 0) {
    float randomY = (float)GetRandomValue(1, 10) / 10.;
    g.ball.vel.y = -g.ball.vel.y * 0.8;
    g.ball.vel.x = g.ball.vel.x * 0.8 + randomY;
  }

  if (g.ball.pos.x < 0 && g.ball.vel.x < 0) {
    g.ball.vel.y = -g.ball.vel.y * 0.8;
    g.ball.vel.x = -g.ball.vel.x * 0.8;
  }

  if (g.ball.pos.x > 800 && g.ball.vel.x > 0) {
    g.ball.vel.y = g.ball.vel.y * 0.9;
    g.ball.vel.x = -g.ball.vel.x * 0.8;
  }

  for (int i = 0; i < ARR_LEN(g.blocks); i++) {
    block_t *block = &g.blocks[i];

    Rectangle block_rect = (Rectangle){
        .x = (*block).pos.x,
        .y = (*block).pos.y,
        .width = BLOCK_SIZE.x,
        .height = BLOCK_SIZE.y,
    };
    if (CheckCollisionRecs(block_rect, ball_rect) &&
        (*block).is_alive == true) {
      float randomY = (float)GetRandomValue(1, 10) / 10.;
      g.ball.vel.y = -g.ball.vel.y * 0.8;
      g.ball.vel.x = g.ball.vel.x * 0.8 * randomY;
      (*block).is_alive = false;
      particle_spawn(
          (Vector2){
              .x = 50,
              .y = 50,
          },
          g.ball.pos, 2.);
      particle_spawn(
          (Vector2){
              .x = 50,
              .y = -50,
          },
          g.ball.pos, 2.);
      particle_spawn(
          (Vector2){
              .x = -50,
              .y = 50,
          },
          g.ball.pos, 2.);
      particle_spawn(
          (Vector2){
              .x = -50,
              .y = -50,
          },
          g.ball.pos, 2.);
    }
  }

  if (g.ball.particle_timer < 0) {
    particle_spawn(Vector2Negate(Vector2Scale(g.ball.vel, 0.2)), g.ball.pos,
                   1.f);
    g.ball.particle_timer = 0.15;
  } else {
    g.ball.particle_timer -= delta;
  }
  g.ball.vel =
      Vector2Clamp(g.ball.vel, Vector2Negate(BALL_MAX_SPEED), BALL_MAX_SPEED);
}

void breakout_update() {
  SetRandomSeed((unsigned int)GetTime()); // Hehe
  switch (g.state) {
  case WIN:
  case LOSE:
    if (IsKeyPressed(KEY_SPACE)) {
      breakout_init();
      g.state = PAUSED;
    }
    break;
  case PAUSED:
    if (IsKeyPressed(KEY_SPACE)) {
      g.state = RUNNING;
    }
    break;
  case RUNNING:
    if (IsKeyPressed(KEY_SPACE)) {
      g.state = PAUSED;
    }

    if (g.lives < 0) {
      g.state = LOSE;
    }
    update_ball();
    particle_update();
    update_player();
    break;
  }
}

void breakout_render() {
  for (int i = 0; i < ARR_LEN(g.particles); i++) {
    if (g.particles[i].active) {
      Color color = (Color){130, 130, 130,
                            g.particles[i].lifetime /
                                g.particles[i].original_lifetime * 255};
      DrawRectanglePro(
          (Rectangle){
              .x = g.particles[i].pos.x + g.particles[i].size.x / 2.f,
              .y = g.particles[i].pos.y + g.particles[i].size.y / 2.f,
              .width = g.particles[i].size.x,
              .height = g.particles[i].size.y,
          },
          (Vector2){
              .x = g.particles[i].size.x / 2.f,
              .y = g.particles[i].size.y / 2.f,
          },
          65.f * RAD2DEG, GRAY);
    }
  }

  DrawRectangle(g.player.pos.x, g.player.pos.y, PADDLE_SIZE.x, PADDLE_SIZE.y,
                RED);
  DrawRectangle(g.ball.pos.x, g.ball.pos.y, BALL_SIZE.x, BALL_SIZE.y, GREEN);

  for (int i = 0; i < ARR_LEN(g.blocks); i++) {
    block_t *block = &g.blocks[i];
    if ((*block).is_alive == true) {
      DrawRectangle((*block).pos.x, (*block).pos.y, BLOCK_SIZE.x, BLOCK_SIZE.y,
                    BLUE);
    }
  }

  DrawFPS(0, 0);
  char buffer[1024] = {0};
  snprintf(buffer, sizeof(buffer), "Live : %d", g.lives);
  int length = MeasureText(buffer, 24) + 20;
  DrawText(buffer, 800 - length, 0, 24, GREEN);
  switch (g.state) {
  case WIN:
    title_screen_template("YOU WON!");
    break;
  case LOSE:
    title_screen_template("GAME OVER");
    break;
  case PAUSED:
    title_screen_template("GAME PAUSED");
    break;
  case RUNNING:
    break;
  }
}
