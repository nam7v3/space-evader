#include "Game.h"
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
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(!window){
                cerr << "Window couldn't be initialized:" << SDL_GetError();
                exit(1);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer){
                cerr << "Failed to create renderer:" << SDL_GetError();
                exit(1);
        }
        asteroid_texture = loadTexture(renderer, "/home/kieu/courses/LTNC/space-evader/asteroid.png");
        if(!asteroid_texture){
                cerr << "Failed to load texture:" << SDL_GetError();
                exit(1);
        }
        player_texture = loadTexture(renderer, "/home/kieu/courses/LTNC/space-evader/arrow.png");
        if(!player_texture){
                cerr << "Failed to load texture:" << SDL_GetError();
                exit(1);
        }
        background_texture = loadTexture(renderer, "/home/kieu/courses/LTNC/space-evader/background.png");
        if(!background_texture){
                cerr << "Failed to load texture:" << SDL_GetError();
                exit(1);
        }
}

Game::~Game(){
        SDL_DestroyTexture(player_texture);
        SDL_DestroyTexture(asteroid_texture);
        SDL_DestroyTexture(background_texture);
        SDL_DestroyWindow(window);
        SDL_Quit();
        IMG_Quit();
}


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

void Game::new_game(){
        srand(time(NULL));

        SDL_Event e;
        Uint32 current;
        Timer timer(1500);
        Player player;
        AsteroidHandler asteroids;

        float elapsed;
        bool tm = true;

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
                SDL_RenderCopy(renderer, background_texture, NULL, NULL);
                asteroids.render(renderer, asteroid_texture);
                player.render(renderer, player_texture);
                SDL_RenderPresent(renderer);

                for(int i = 0; i < timer.time_interval_elapsed(); ++i)
                        asteroids.add_rand_asteroid();

                if(!player.is_invincible()){
                        float player_px = player.get_px();
                        float player_py = player.get_py();
                        int player_radius = player.get_radius();

                        for(auto &a: asteroids.get_list()){
                                if((a.px - player_px) * (a.px - player_px) + \
                                   (a.py - player_py) * (a.py - player_py) < \
                                   (a.radius + player_radius) * (a.radius + player_radius))
                                        player.hit();
                        }
                        if(player.dead())break;
                }

                elapsed = tick_to_sec(SDL_GetTicks() - current);

                asteroids.update(elapsed);
                player.update(elapsed);

                // Stage 2
                if(timer.get_ticks() > 30000 && tm){
                        timer.set_interval(1000);
                        tm =false;
                }
                SDL_Delay(10);
        }
}
