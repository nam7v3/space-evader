#include "../include/Game.h"

SDL_Texture* Game::load_img_texture(string path)
{
    if (renderer == NULL)
        return NULL;
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

int Game::render_text(string text, int x, int y)
{
    SDL_Texture* texture = load_text_texture(text);
    SDL_Rect dst = { x, y, font_size * (int)text.length(), 2 * font_size };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    return 0;
}

SDL_Texture* Game::load_text_texture(string text)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), defautl_text_color);
    SDL_Texture* texture = NULL;
    if (surface == NULL) {
        cerr << "Couldn't load text:" << TTF_GetError();
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
    font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
        cerr << "Font: " << TTF_GetError();
        exit(1);
    }

    // Import game textures
    for (int i = 0; i < TEXTURE_IMG_NUM; ++i) {
        textures_img[i] = load_img_texture(resources_path + to_string(i) + ".png");
        if (!textures_img[i]) {
            cerr << "Failed to load texture:" << SDL_GetError();
            exit(1);
        }
    }
}

Game::~Game()
{
    for (int i = 0; i < TEXTURE_IMG_NUM; ++i) {
        SDL_DestroyTexture(textures_img[i]);
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
            SDL_RenderCopy(renderer, textures_img[TEXTURE_IMG_MENU], &texture_menu[MENU_PLAY],
                NULL);
        else
            SDL_RenderCopy(renderer, textures_img[TEXTURE_IMG_MENU], &texture_menu[MENU_QUIT],
                NULL);
        if (best_score) {
            render_text("BEST: " + to_string(best_score), 300, 500);
        }
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
                if (menu == MENU_PLAY) {
                    game_state = GAME_STATE_PLAYING;
                    new_game();
                } else
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
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                timer.pause();
                pause();
                timer.unpause();
            }
            player.handle_event(e);
        }
        if (game_state == GAME_STATE_MENU)
            return;

        // Add random asteroid in interval
        for (int i = 0; i < timer.time_interval_elapsed(); ++i)
            asteroids.add_rand_asteroid();

        player.check_asteroid_collision(asteroids.get_list());

        if (player.dead()) {
            best_score = max(score, best_score);
            break;
        }

        score = timer.get_ticks() / 1000;

        asteroids.update(elapsed);
        player.update(elapsed);

        set_stage(stage, timer);
        SDL_RenderClear(renderer);

        // Render Game
        SDL_RenderSetViewport(renderer, &game_screen);
        SDL_RenderCopy(renderer, textures_img[TEXTURE_IMG_BACKGROUND], NULL, NULL);
        asteroids.render(renderer, textures_img[TEXTURE_IMG_ASTEROID]);
        player.render(renderer, textures_img);

        // Render UI
        SDL_RenderSetViewport(renderer, &real_screen);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
        SDL_RenderDrawLine(renderer, GAME_WIDTH, 0, GAME_WIDTH, GAME_HEIGHT);
        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0);

        render_text("SCORE: " + to_string(score), GAME_WIDTH + UI_PADDING, 0);
        render_text("LIVES: " + to_string(player.get_lives()), GAME_WIDTH + UI_PADDING, (font_size * 2 + UI_PADDING));
        render_text("STAGE: " + to_string(stage), GAME_WIDTH + UI_PADDING, (font_size * 2 + UI_PADDING) * 2);
        render_text("MOVEMENT", GAME_WIDTH + UI_PADDING, (font_size * 2 + UI_PADDING) * 5);

        SDL_RenderCopy(renderer, textures_img[TEXTURE_IMG_WASD], NULL, &wasd_pos);
        render_text("Pause", GAME_WIDTH + UI_PADDING, GAME_HEIGHT - 200);
        SDL_RenderCopy(renderer, textures_img[TEXTURE_IMG_ESC], NULL, &esc_pos);
        SDL_RenderPresent(renderer);

        SDL_Delay(fps_sleep(timer.sec_since_mark()));
    }
    return;
}

void Game::set_stage(int& stage, Timer& timer)
{
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
    if (timer.get_ticks() > 35000 && stage == 2) {
        timer.set_interval(500);
        stage = 3;
    }
    // Stage 4
    if (timer.get_ticks() > 50000 && stage == 3) {
        timer.set_interval(200);
        stage = 4;
    }
}

void Game::pause()
{
    SDL_Event e;
    while (true) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textures_img[TEXTURE_IMG_PAUSE], NULL, NULL);
        SDL_RenderPresent(renderer);
        while (SDL_WaitEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_state = GAME_STATE_MENU;
                    return;
                case SDLK_RETURN:
                    game_state = GAME_STATE_PLAYING;
                    return;
                }
            }
        }
    }
}

void Game::save_high_score(int score)
{
}
