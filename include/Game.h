#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <random>
#include <string>
#include <chrono>

#include "Asteroid.h"
#include "Player.h"
#include "Timer.h"
using namespace std;


const int GAME_HEIGHT = 700;
const int GAME_WIDTH = 600;
const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 800;


#define FPS 60
// Milliseconds to sleep
#define fps_sleep(a) (1000/FPS-(a)/1000)

const SDL_Rect game_screen = {
        0,
        0,
    GAME_WIDTH,
    GAME_HEIGHT
};

const SDL_Rect real_screen = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

#define TEXTURE_EXPLOSION 0
#define TEXTURE_BACKGROUND 1
#define TEXTURE_PLAYER 2
#define TEXTURE_ARROW 3
#define TEXTURE_ASTEROID 4
#define TEXTURE_MENU 5
#define TEXTURE_NUM 6


#define MENU_PLAY 0
#define MENU_QUIT 1

const SDL_Rect texture_menu[] = {
        SDL_Rect{ 0, 0, 700, 700 },
        SDL_Rect{ 700, 0, 700, 700 }
};

const string resources_path = "resources/";
const string font_path = resources_path + "font.ttf";
const int font_height = 16;


class Game {
public:
    Game();
    ~Game();
    void new_game();
    void main_menu();
    SDL_Texture* load_img_texture(string path);
    int render_text(string text, SDL_Rect dst, SDL_Color c, TTF_Font *f);
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[TEXTURE_NUM] = { NULL, NULL, NULL, NULL, NULL };
    SDL_Texture* font_texture;
    TTF_Font* font;
    bool running;
    int menu = 0;
};
