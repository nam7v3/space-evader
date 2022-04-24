#include "Player.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <cmath>

Player::Player(){
        px = GAME_WIDTH * 0.5f;
        py = GAME_HEIGHT * 0.5f;

        dx = 0.0; dy = -1.0;
        vx = 0.0; vy = 0.0;
        ax = 0.0; ay = 0.0;

        radius = DEFAULT_RADIUS;
        angle = 0;

        lives = 3;
};

void Player::handle_event(SDL_Event &e){
        if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                        case SDLK_d:
                                state[MOVE_RIGHT] = true;
                                break;
                        case SDLK_a:
                                state[MOVE_LEFT] = true;
                                break;
                        case SDLK_w:
                                state[MOVE_FOWARD] = true;
                                break;
                        case SDLK_s:
                                state[MOVE_BACKWARD] = true;
                                break;
                        }
        }
        if(e.type == SDL_KEYUP){
                switch (e.key.keysym.sym){
                        case SDLK_d:
                                state[MOVE_RIGHT] = false;
                                break;
                        case SDLK_a:
                                state[MOVE_LEFT] = false;
                                break;
                        case SDLK_w:
                                state[MOVE_FOWARD] = false;
                                break;
                        case SDLK_s:
                                state[MOVE_BACKWARD] = false;
                                break;
                        }
        }
};

void Player::toggle_invincible(){
        if(is_invincible()){
                invincible_timer.stop();
        }else {
                invincible_timer.start();
        }
}

bool Player::is_invincible(){
        if(invincible_timer.get_ticks() < INVINCIBLE_TIME && invincible_timer.is_started()) {
                return true;
        }
        return false;
}

void Player::hit(){
        if(!is_invincible()){
                toggle_invincible();
                explosion = 0;
                lives--;
        }
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
        if(state[MOVE_FOWARD])move_foward();
        if(state[MOVE_BACKWARD])move_backward();
        if(state[MOVE_LEFT])turn(-DELTA_ANGLE);
        if(state[MOVE_RIGHT])turn(DELTA_ANGLE);

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

const float Player::get_px(){
        return px;
}

const float Player::get_py(){
        return py;
}

const float Player::get_vx(){
        return vx;
}

const float Player::get_vy(){
        return vy;
}

const int Player::get_radius(){
        return radius;
}

void Player::render(SDL_Renderer *renderer, SDL_Texture *player_texture, SDL_Texture *arrow_texture, SDL_Texture *explosion_texture){
        SDL_Rect player_dst = {(int)px - radius, (int)py - radius, radius * 2 , radius * 2};
        SDL_Rect arrow_dst = {(int)px + (int)(dx * 100) - 24, (int)py + (int)(dy * 100) - 12, 48, 24};
        SDL_Rect src = {(3 - lives) * 85, 0, 85, 87};

        if(is_invincible()){
                SDL_SetTextureAlphaMod(player_texture, 50);
        }else {
                SDL_SetTextureAlphaMod(player_texture, 255);
        }
        if(SDL_RenderCopyEx(renderer, player_texture, &src, &player_dst, angle, NULL, SDL_FLIP_NONE) < 0){
                cerr << "Couldn't render player" <<  SDL_GetError();
        }

        if(SDL_RenderCopyEx(renderer, arrow_texture, NULL, &arrow_dst, angle, NULL, SDL_FLIP_NONE) < 0){
                cerr << "Couldn't render player" <<  SDL_GetError();
        }
        if(explosion >= 0){
                SDL_Rect dat = {explosion * 96, 0 , 96, 96};
                SDL_Rect dis = {(int)px - 100, (int)py -100 , 200, 200};
                if(SDL_RenderCopy(renderer, explosion_texture, &dat, &dis) < 0){
                        cerr << "Couldn't render player" <<  SDL_GetError();
                }
                cout << explosion << '\n' ;
                if(explosion == 11) explosion = -1;
                else explosion++;
        }
};
