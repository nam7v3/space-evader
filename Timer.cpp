#include "Timer.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

Timer::Timer(Uint32 t): interval(t), start_ticks(0), paused_ticks(0), time_slot(0), started(false), paused(false){};
Timer::Timer(): interval(0), start_ticks(0), paused_ticks(0), time_slot(0), started(false), paused(false){};

void Timer::start(){
        started = true;
        paused = false;
        time_slot = 0;
        start_ticks = SDL_GetTicks();
        paused_ticks = 0;
}
void Timer::stop(){
        started = false;
        paused = false;
        start_ticks = 0;
        paused_ticks = 0;
}

void Timer::pause(){
        if(started && !paused){
                paused = true;
                paused_ticks = SDL_GetTicks() - start_ticks;
                start_ticks = 0;
        }
}
void Timer::unpause(){
        if(started && paused){
                paused = false;
                start_ticks = SDL_GetTicks() - paused_ticks;
                paused_ticks = 0;
        }
}

Uint32 Timer::get_ticks(){
        if(!started) return 0;
        if(paused) return paused_ticks;
        else return SDL_GetTicks() - start_ticks;
}

int Timer::time_interval_elapsed(){
        Uint32 ntimes = std::max(get_ticks() / interval, time_slot);
        int ret = ntimes - time_slot;
        time_slot = ntimes;
        return ret;
}

void Timer::set_interval(int i){
        time_slot = get_ticks() / i;
        interval = i;
}

bool Timer::is_started(){
        return started;
}

bool Timer::is_paused(){
        return paused;
}


