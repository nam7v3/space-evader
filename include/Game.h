#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <random>
#include <string>

#include "../include/Asteroid.h"
#include "../include/Player.h"
#include "../include/Timer.h"
using namespace std;

const int GAME_HEIGHT = 700;
const int GAME_WIDTH = 600;
const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 800;

#define FPS 60
// Milliseconds to sleep
#define fps_sleep(a) (1000 / FPS - (a) / 1000)

const SDL_Rect game_screen = { 0,
    0,
    GAME_WIDTH,
    GAME_HEIGHT };

const SDL_Rect real_screen = {
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

#define TEXTURE_IMG_EXPLOSION 0
#define TEXTURE_IMG_BACKGROUND 1
#define TEXTURE_IMG_PLAYER 2
#define TEXTURE_IMG_ARROW 3
#define TEXTURE_IMG_ASTEROID 4
#define TEXTURE_IMG_MENU 5
#define TEXTURE_IMG_WASD 6
#define TEXTURE_IMG_PAUSE 7
#define TEXTURE_IMG_ESC 8 
#define TEXTURE_IMG_NUM 9

#define TEXTURE_TEXT_SCORE 0
#define TEXTURE_TEXT_STAGE 1
#define TEXTURE_TEXT_LIVES 2
#define TEXTURE_TEXT_MOVEMENT 3
#define TEXTURE_TEXT_PAUSE 4
#define TEXTURE_TEXT_NUM 5

const string text[] = {
    "SCORE: ", "STAGE: ", "LIVES: ", "MOVEMENT", "PAUSE",
};

#define UI_PADDING 10

const SDL_Rect text_pos[] = {
    {GAME_WIDTH + UI_PADDING, 0, 70, 70},
    {GAME_WIDTH + UI_PADDING, 70 + UI_PADDING, 70, 70},
    {GAME_WIDTH + UI_PADDING, (70 + UI_PADDING) * 2, 70, 70},
    {GAME_WIDTH + UI_PADDING, (70 + UI_PADDING) * 3, 160, 70},
    {GAME_WIDTH + UI_PADDING, GAME_HEIGHT / 2 + 110, 70, 70},
};

const SDL_Rect wasd_pos = {GAME_WIDTH + 20, GAME_HEIGHT / 2 - 40, 150, 150};

const SDL_Rect stat_pos[] = {
    { GAME_WIDTH + UI_PADDING + 70, 0, 70, 70 },
    { GAME_WIDTH + UI_PADDING + 70, 70 + UI_PADDING, 70, 70 },
    { GAME_WIDTH + UI_PADDING + 70, (70 + UI_PADDING) * 2, 70, 70 },
};


#define MENU_PLAY 0
#define MENU_QUIT 1

#define GAME_STATE_MENU 0
#define GAME_STATE_PAUSE 1
#define GAME_STATE_PLAYING 2
const SDL_Rect texture_menu[]
    = { { 0, 0, 700, 700 }, { 700, 0, 700, 700 } };


const SDL_Color defautl_text_color = { 0xff, 0xff, 0xff };

const string resources_path = "resources/";
const string font_path = resources_path + "font.ttf";
const int font_height = 16;

class Game {
public:
    Game();
    ~Game();

    void new_game();
    void main_menu();
    void pause();
    void set_stage(int &stage, Timer &timer);

    SDL_Texture* load_img_texture(string path);
    int render_text(string text, const SDL_Rect& dst);
    SDL_Texture* load_text_texture(string text);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures_img[TEXTURE_IMG_NUM] = { NULL };
    SDL_Texture* textures_text[TEXTURE_TEXT_NUM] = { NULL };
    TTF_Font* font;
  int game_state = GAME_STATE_MENU;
    bool running;
    int menu = 0;
};
