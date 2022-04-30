#include "Game.h"

SDL_Texture* loadTexture(SDL_Renderer* r, std::string path){
        SDL_Surface* surface = IMG_Load(path.c_str());
        SDL_Texture* texture = NULL;
        if(surface == NULL){
                cerr << "Could't load image:" << IMG_GetError();
                return NULL;
        }
        texture = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
        return texture;
}

Game::Game(){
        running = true;
        if(SDL_Init(SDL_INIT_VIDEO) < 0){
                cerr << "SDL couldn't be initialized:" << SDL_GetError();
                exit(1);
        }

        if((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG){
                cerr << "SDL_image couldn't be initialized:" << IMG_GetError();
                exit(1);
        }
        window = SDL_CreateWindow("Space Evader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(!window){
                cerr << "Window couldn't be initialized:" << SDL_GetError();
                exit(1);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer){
                cerr << "Failed to create renderer:" << SDL_GetError();
                exit(1);
        }
        for(int i = 0; i < TEXTURE_NUM; ++i){
                textures[i] = loadTexture(renderer, resources_path + to_string(i) + ".png");
                if(!textures[i]){
                        cerr << "Failed to load texture:" << SDL_GetError();
                        exit(1);
                }
        }
}

Game::~Game(){
        for(int i = 0; i < TEXTURE_NUM; ++i){
                SDL_DestroyTexture(textures[i]);
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
        IMG_Quit();
}

void Game::main_menu(){
        while(true){
                while(SDL_WaitEvent(&e)){
                        if(e.type == SDL_QUIT){
                                return;
                        }
        }
}
void Game::new_game(){
        srand(time(NULL));

        SDL_Event e;
        Uint32 current;
        Timer timer(1500);
        Player player;
        AsteroidHandler asteroids;

        float elapsed;
        bool tm = true;
        point = 0;

        SDL_ShowCursor(SDL_DISABLE);
        timer.start();
        while(true){
                current = timer.get_ticks();
                while(SDL_PollEvent(&e)){
                        if(e.type == SDL_QUIT){
                                return;
                        }
                        player.handle_event(e);
                }


                SDL_RenderClear(renderer);

                // Draw the border

                // Draw the gaming screen
                SDL_RenderSetViewport(renderer, &game_screen);

                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
                SDL_RenderDrawRect(renderer, NULL);
                SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                // Background
                SDL_RenderCopy(renderer, textures[TEXTURE_BACKGROUND], NULL, NULL);
                asteroids.render(renderer, textures[TEXTURE_ASTEROID]);
                player.render(renderer, textures);

                SDL_RenderPresent(renderer);

                for(int i = 0; i < timer.time_interval_elapsed(); ++i)
                        asteroids.add_rand_asteroid();

                // Player-Asteroid collision

                float player_px = player.get_px();
                float player_py = player.get_py();
                int player_radius = player.get_radius();

                player.is_invincible();
                for(auto &a: asteroids.get_list()){
                        if((a.px - player_px) * (a.px - player_px) + (a.py - player_py) * (a.py - player_py) < (a.radius + player_radius) * (a.radius + player_radius)){
                                player.hit();

                                float dis = sqrt((a.px - player_px) * (a.px - player_px) + (a.py - player_py) * (a.py - player_py));

                                a.px += (a.radius + player_radius - dis)* (a.px - player_px) / dis;
                                a.py += (a.radius + player_radius - dis) * (a.py - player_py) / dis;

                                a.vx = (a.px - player_px) * 5;
                                a.vy = (a.py - player_py) * 5;
                        }

                }
                if(player.dead())break;

                elapsed = tick_to_sec(SDL_GetTicks() - current);

                asteroids.update(elapsed);
                player.update(elapsed);

                // Stage 2
                if(timer.get_ticks() > 30000 && tm){
                        timer.set_interval(1000);
                        tm = false;
                }
                SDL_Delay(30);
        }
}
