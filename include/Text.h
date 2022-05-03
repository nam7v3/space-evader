#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include <string>
using namespace std;
class Text {
public:
    Text();
    ~Text();

    SDL_Texture* get_text_texture(SDL_Renderer *r, string text);
    void render_text(SDL_Renderer* r, SDL_Rect dst, SDL_Color color, string text);
private:
    TTF_Font* font;
}
