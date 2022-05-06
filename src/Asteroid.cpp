#include "../include/Asteroid.h"
#include "../include/Game.h"
#include <SDL2/SDL_render.h>

bool outside(const Asteroid& a)
{
    return a.px < -PADDING || a.px > GAME_WIDTH + PADDING || a.py < -PADDING || a.py > GAME_HEIGHT + PADDING;
}

void AsteroidHandler::add_rand_asteroid()
{
    Asteroid a;
    int randid;

    a.radius = rand() % DEFAULT_ASTEROID_RADIUS + 20;
    a.angle = 0;
    a.speed = ASTEROID_SPEED;
    a.delta_angle = (rand() % 2 ? -1.0 : 1.0) * 10.0f / a.radius;

    randid = rand() % 4;

    if (randid == 0) {
        // Spawn an asteroid on right border
        a.px = GAME_WIDTH + PADDING;
        a.py = rand() % GAME_HEIGHT;
        a.vx = -1.0f * a.speed;
        a.vy = 1.0f / (std::rand() % 5 + 1) * a.speed;
    }
    if (randid == 1) {
        // Spawn an asteroid on left border
        a.px = -PADDING;
        a.py = rand() % GAME_HEIGHT;
        a.vx = 1.0f * ASTEROID_SPEED;
        a.vy = 1.0f / (rand() % 5 + 1) * a.speed;
    }
    if (randid == 2) {
        // Spawn an asteroid on bottom border
        a.px = rand() % GAME_WIDTH;
        a.py = GAME_HEIGHT + PADDING;
        a.vx = 1.0f / (rand() % 5 + 1) * a.speed;
        a.vy = -1.0f * a.speed;
    }
    if (randid == 3) {
        // Spawn an asteroid on top border
        a.px = rand() % GAME_WIDTH;
        a.py = -PADDING;
        a.vx = 1.0f / (rand() % 5) * a.speed;
        a.vy = 1.0f * a.speed;
    }

    a.id = ++asteroid_num;

    asteroids.push_back(a);
};

void AsteroidHandler::add_asteroid(Asteroid a)
{
    asteroids.push_back(a);
}

list<Asteroid>& AsteroidHandler::get_list()
{
    return asteroids;
}

void AsteroidHandler::update(float t)
{
    stack<pair<Asteroid&, Asteroid&>> collide_pair;
    float dis, overlap;
    for (auto& a : asteroids) {
        for (auto& b : asteroids) {
            if (a.id != b.id && asteroid_collide(a, b)) {
                dis = asteroid_distance(a, b);

                overlap = 0.5f * (dis - a.radius - b.radius);

                a.px -= overlap * (a.px - b.px) / dis;
                a.py -= overlap * (a.py - b.py) / dis;

                b.px += overlap * (a.px - b.px) / dis;
                b.py += overlap * (a.py - b.py) / dis;

                collide_pair.push({ a, b });
            }
        }
    }
    while (collide_pair.size()) {
        Asteroid& a = collide_pair.top().first;
        Asteroid& b = collide_pair.top().second;

        dis = asteroid_distance(a, b);

        float nx = (b.px - a.px) / dis;
        float ny = (b.py - a.py) / dis;
        float p = 2.0 * (nx * a.vx + ny * a.vy - nx * b.vx - ny * b.vy) / (a.radius + b.radius);
        a.vx = a.vx - p * b.radius * nx;
        a.vy = a.vy - p * b.radius * ny;

        b.vx = b.vx + p * a.radius * nx;
        b.vy = b.vy + p * a.radius * ny;

        collide_pair.pop();
    }
    for (auto& a : asteroids) {
        a.px = a.px + a.vx * t;
        a.py = a.py + a.vy * t;

        a.angle = a.angle + a.delta_angle;

        if (a.angle > 360.0)
            a.angle -= 360;
        if (a.angle < 0.0)
            a.angle -= 360;
    }
    asteroids.remove_if(outside);
}

void AsteroidHandler::render(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_Rect dst;
    if (!renderer || !texture) {
        return;
    }
    for (auto& a : asteroids) {
        dst.x = a.px - a.radius;
        dst.y = a.py - a.radius;
        dst.w = a.radius * 2;
        dst.h = a.radius * 2;

        if (SDL_RenderCopyEx(renderer, texture, NULL, &dst, a.angle, NULL, SDL_FLIP_NONE) < 0) {
            cerr << "Could't render asteroid" << SDL_GetError();
        }
    }
}
