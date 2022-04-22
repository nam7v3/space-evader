#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <random>
#include <iostream>
#include <string>

#include "Asteroid.h"
#include "Timer.h"
#include "Player.h"
using namespace std;

#define tick_to_sec(a) (a)/20.0/1000.0

const int GAME_HEIGHT = 600;
const int GAME_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 600;

const int FPS = 15;

const string res_path = "/home/kieu/courses/LTNC/space-evader/resources";

SDL_Texture* loadTexture(SDL_Renderer* r, string path);

class Game {
        public:
                Game();
                ~Game();
                void new_game();
                void main_menu();

        private:
                SDL_Window *window;
                SDL_Renderer *renderer;
                SDL_Texture *player_texture, *asteroid_texture, \
                        *menu_texture, *background_texture;

                bool running;
};
