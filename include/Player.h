#pragma once
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include <iostream>

#include "Game.h"
#include "Asteroid.h"
#include "Timer.h"
using namespace std;

#define DEFAULT_LIVES 3
#define DEFAULT_RADIUS 30
#define INVINCIBLE_TIME 2000
#define DELTA_ANGLE 3.0
#define VELOCITY 3.6
#define ACCELERATION 0.05

#define MOVE_FOWARD 0
#define MOVE_BACKWARD 1
#define MOVE_RIGHT 2
#define MOVE_LEFT 3

#define PI 3.14159265358979323846
#define deg_to_rad(a) a / 180 * PI

class Player {
public:
    Player();
    void handle_event(SDL_Event& e);

    void turn(float delta_angle);
    void move_foward();
    void move_backward();

    void toggle_invincible();
    void hit();
    bool dead();
    void check_asteroid_collision(list<Asteroid> &a);

    bool is_invincible();

    const float get_px();
    const float get_py();
    const float get_vx();
    const float get_vy();
    const int get_radius();

    const int get_lives();
    void update(float t);

    void render(SDL_Renderer* r, SDL_Texture** textures);

private:
    float px, py; // Position of player
    float dx, dy; // Direction player pointing to
    float vx, vy; // Velocity of player
    float ax, ay; // Acceleration of player

    int radius;
    float angle;

    int invincible_animation_state = 0;
    int explosion = -1;

    int lives;
    bool state[4] = { 0, 0, 0, 0 };
    Timer invincible_timer;
};
