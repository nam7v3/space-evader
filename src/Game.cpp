#include "../include/Game.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* Game::load_img_texture(string path)
{
    if(renderer == NULL) return NULL;
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = NULL;
    if (surface == NULL) {
        cerr << "Could't load image:" << IMG_GetError();
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Game::Game()
{
    running = true;
    // Initialize library
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL: " << SDL_GetError();
        exit(1);
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        cerr << "SDL_image: " << IMG_GetError();
        exit(1);
    }
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf: " << TTF_GetError();
        exit(1);
    }
    // Create window
    window = SDL_CreateWindow("Space Evader", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        cerr << "Window: " << SDL_GetError();
        exit(1);
    }

    // Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer: " << SDL_GetError();
        exit(1);
    }
    // Import font
    font = TTF_OpenFont(font_path.c_str(), font_height);
    if (!font) {
        cerr << "Font: " << TTF_GetError();
        exit(1);
    }

    // Import game textures
    for (int i = 0; i < TEXTURE_NUM; ++i) {
        textures[i] = load_img_texture(resources_path + to_string(i) + ".png");
        if (!textures[i]) {
            cerr << "Failed to load texture:" << SDL_GetError();
            exit(1);
        }
    }
}

Game::~Game()
{
    for (int i = 0; i < TEXTURE_NUM; ++i) {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::main_menu()
{
    srand(time(NULL));
    SDL_Event e;
    while (running) {
        if (menu == MENU_PLAY)
          SDL_RenderCopy(renderer, textures[TEXTURE_MENU], &texture_menu[MENU_PLAY],
                NULL);
        else
            SDL_RenderCopy(renderer, textures[TEXTURE_MENU], &texture_menu[MENU_QUIT],
                NULL);
        SDL_RenderPresent(renderer);
        SDL_WaitEvent(&e);
        if (e.type == SDL_QUIT) {
            return;
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_w) {
                menu = MENU_PLAY;
            }
            if (e.key.keysym.sym == SDLK_s) {
                menu = MENU_QUIT;
            }
            if (e.key.keysym.sym == SDLK_RETURN) {
                if (menu == MENU_PLAY)
                    new_game();
                else
                    return;
            }
        }
    }
}

void Game::new_game()
{

    SDL_Event e;
    Uint32 current;
    Timer timer(1500);
    Player player;
    AsteroidHandler asteroids;
    SDL_Texture* text = NULL;
    SDL_Surface* surface = NULL;

    float elapsed;
    int stage = 0;
    int score = 0;


    SDL_ShowCursor(SDL_DISABLE);
    timer.start();
    while (running) {
        elapsed = timer.sec_since_mark();
        timer.mark_timer();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
              running = false;
            }
            player.handle_event(e);
        }

        // Add random asteroid in interval
        for (int i = 0; i < timer.time_interval_elapsed(); ++i)
            asteroids.add_rand_asteroid();


        // Player-Asteroid collision
        float player_px = player.get_px();
        float player_py = player.get_py();
        int player_radius = player.get_radius();

        player.is_invincible();
        for (auto& a : asteroids.get_list()) {
            if ((a.px - player_px) * (a.px - player_px) + (a.py - player_py) * (a.py - player_py) < (a.radius + player_radius) * (a.radius + player_radius)) {
                player.hit();

                float dis = sqrt((a.px - player_px) * (a.px - player_px) + (a.py - player_py) * (a.py - player_py));

                a.px += (a.radius + player_radius - dis) * (a.px - player_px) / dis;
                a.py += (a.radius + player_radius - dis) * (a.py - player_py) / dis;

                a.vx = (a.px - player_px) * 0.04;
                a.vy = (a.py - player_py) * 0.04;
            }
        }
        if (player.dead())
            break;

        score = timer.get_ticks() / 1000;

        asteroids.update(elapsed);
        player.update(elapsed);

        // Stage 1
        if (timer.get_ticks() > 15000 && stage == 0) {
            timer.set_interval(1000);
            stage = 1;
        }
        // Stage 2
        if (timer.get_ticks() > 25000 && stage == 1) {
          timer.set_interval(800);
          stage = 2;
        }
        // Stage 3
        if (timer.get_ticks() > 35000 && stage == 1) {
          timer.set_interval(500);
          stage = 2;
        }

        SDL_RenderClear(renderer);

        // Game screen
        SDL_RenderSetViewport(renderer, &game_screen);
        SDL_RenderCopy(renderer, textures[TEXTURE_BACKGROUND], NULL, NULL);
        SDL_RenderCopy(renderer, text, NULL, NULL);
        asteroids.render(renderer, textures[TEXTURE_ASTEROID]);
        player.render(renderer, textures);

        SDL_RenderSetViewport(renderer, &real_screen);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
        SDL_RenderDrawLine(renderer, GAME_WIDTH, 0, GAME_WIDTH, GAME_HEIGHT);
        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0);

        SDL_RenderPresent(renderer);

        SDL_Delay( fps_sleep(timer.sec_since_mark()) );
    }
    return;
}
