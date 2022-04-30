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
const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 700;

const SDL_Rect game_screen = { \
        (SCREEN_WIDTH - GAME_WIDTH)/2, \
        (SCREEN_HEIGHT - GAME_HEIGHT)/2, \
        GAME_WIDTH, \
        GAME_HEIGHT \
};

#define TEXTURE_EXPLOSION   0
#define TEXTURE_BACKGROUND  1
#define TEXTURE_PLAYER      2
#define TEXTURE_ARROW       3
#define TEXTURE_ASTEROID    4
#define TEXTURE_NUM         5

const int FPS = 15;

const string resources_path = "resources/";

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
                SDL_Texture *textures[TEXTURE_NUM] = {NULL, NULL, NULL, NULL, NULL};
                bool running;
                int point = 0;
};
