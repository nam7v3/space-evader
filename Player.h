#pragma once
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include <iostream>

#include "Game.h"
#include "Timer.h"
using namespace std;

#define DEFAULT_LIVES  3
#define DEFAULT_RADIUS 20
#define INVINCIBLE_TIME 2000
#define DELTA_ANGLE 10.0
#define VELOCITY 230
#define ACCELERATION 130

#define PI 3.14159265358979323846
#define deg_to_rad(a) a/180*PI

class Player{
        public:
                Player();
                void handle_event(SDL_Event &e);

                void turn(float delta_angle);
                void move_foward();
                void move_backward();

                bool is_invincible();
                void hit();
                bool dead();

                void update(float t);

                void render(SDL_Renderer *r, SDL_Texture *t);
        private:
                float px, py; // Position of player
                float dx, dy; // Direction player pointing to
                float vx, vy; // Velocity of player
                float ax, ay; // Acceleration of player
                int radius;
                float angle;

                int lives;
                bool invincible;
                Timer invincible_timer;
};

