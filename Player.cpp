#include "Player.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <cmath>

Player::Player(){
        px = GAME_WIDTH * 0.5f;
        py = GAME_HEIGHT * 0.5f;

        dx = 1.0; dy = 0.0;
        vx = 0.0; vy = 0.0;
        ax = 0.0; ay = 0.0;

        radius = DEFAULT_RADIUS;
        angle = 0;

        lives = 3;
        invincible = false;

};

void Player::handle_event(SDL_Event &e){
        /* if(keyboard[SDLK_a]){ */
        /*         turn(-DELTA_ANGLE); */
        /* } */
        /* if(keyboard[SDLK_d]){ */
        /*         turn(DELTA_ANGLE); */
        /* } */
        /* if(keyboard[SDLK_w]){ */
        /*         move_backward(); */
        /* } */
        /* if(keyboard[SDLK_s]){ */
        /*         move_backward(); */
        /* } */
        auto keyboard = SDL_GetKeyboardState(NULL);
        if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
        switch (e.key.keysym.sym){
                case SDLK_d:
                        turn(DELTA_ANGLE);
                        /* if(keyboard[SDLK_w]){ */
                        /*         move_foward(); */
                        /* } */
                        /* if(keyboard[SDLK_s]){ */
                        /*         move_backward(); */
                        /* } */
                        break;
                case SDLK_a:
                        turn(-DELTA_ANGLE);
                        /* if(keyboard[SDLK_w]){ */
                        /*         move_foward(); */
                        /* } */
                        /* if(keyboard[SDLK_s]){ */
                        /*         move_backward(); */
                        /* } */
                        break;
                case SDLK_w:
                        move_foward();
                        /* if(keyboard[SDLK_a]){ */
                        /*         turn(-DELTA_ANGLE); */
                        /* } */
                        /* if(keyboard[SDLK_d]){ */
                        /*         turn(DELTA_ANGLE); */
                        /* } */
                        break;
                case SDLK_s:
                        move_backward();
                        /* if(keyboard[SDLK_a]){ */
                        /*         turn(-DELTA_ANGLE); */
                        /* } */
                        /* if(keyboard[SDLK_d]){ */
                        /*         turn(DELTA_ANGLE); */
                        /* } */
                        break;
                }
        }
};

bool Player::is_invincible(){
        if(invincible_timer.get_ticks() < INVINCIBLE_TIME && invincible_timer.is_started()) {
                invincible_timer.stop();
                return true;
        }
        return false;
}

void Player::hit(){
        invincible_timer.start();
        lives--;
}

bool Player::dead(){
        if(lives > 0)return false;
        return true;
}

void Player::turn(float delta_angle){
        float tempx, tempy;
        tempx = dx * cos(deg_to_rad(delta_angle)) - dy * sin(deg_to_rad(delta_angle));
        tempy = dx * sin(deg_to_rad(delta_angle)) + dy * cos(deg_to_rad(delta_angle));

        dx = tempx;
        dy = tempy;

        angle += delta_angle;
        if(angle > 360.0) angle -= 360;
        if(angle < 0.0) angle += 360;
}

void Player::move_foward(){
        vx = dx * VELOCITY;
        vy = dy * VELOCITY;
        ax = -dx * ACCELERATION;
        ay = -dy * ACCELERATION;
}

void Player::move_backward(){
        vx = -dx * VELOCITY;
        vy = -dy * VELOCITY;
        ax = dx * ACCELERATION;
        ay = dy * ACCELERATION;
}
void Player::update(float t){
        if (vx * (vx + ax * t) < 0){
                vx = 0;
                ax = 0;
        } else vx = vx + ax * t;
        if(vy * (vy + ay * t) < 0){
                vy = 0;
                ay = 0;
        } else vy = vy + ay * t;

        if(px + vx * t + radius < GAME_WIDTH && px + vx * t - radius > 0) px = px + vx * t;
        if(py + vy * t + radius < GAME_HEIGHT && py + vy * t - radius > 0) py = py + vy * t;
}

void Player::render(SDL_Renderer *renderer, SDL_Texture *texture){
        SDL_Rect dst = {(int)px - radius, (int)py - radius, radius * 2 , radius * 2};

        if(SDL_RenderCopyEx(renderer, texture, NULL, &dst, angle, NULL, SDL_FLIP_NONE) < 0){
                cerr << "Couldn't render player" <<  SDL_GetError();
        }
};
