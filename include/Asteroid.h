#pragma once

#include <list>
#include <stack>
#include <utility>
#include <cmath>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

using namespace std;

#define DEFAULT_ASTEROID_RADIUS 60
#define ASTEROID_SPEED 3
#define PADDING 100

#define asteroid_collide(a,b) ((a.px - b.px) * (a.px - b.px) + (a.py - b.py) * (a.py - b.py) < (a.radius + b.radius) * (a.radius + b.radius))
#define asteroid_distance(a,b) sqrt((a.px - b.px) * (a.px - b.px) + (a.py - b.py) * (a.py - b.py))

typedef struct {
    float px, py;
    float ax, ay;
    float vx, vy;
    int radius;
    int speed;
    double angle, delta_angle;
    int id;
} Asteroid;

class AsteroidHandler {
public:
    void add_rand_asteroid();
    void add_asteroid(Asteroid a);

    void erase_asteroid(int id);

    void render(SDL_Renderer* r, SDL_Texture* t);
    void update(float time);
    list<Asteroid>& get_list();

private:
    list<Asteroid> asteroids;
    int asteroid_num = 0;
};
