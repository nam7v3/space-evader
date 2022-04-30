#pragma once
#include <algorithm>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

class Timer{
        public:
                Timer();
                Timer(Uint32 interval);

                void start();
                void stop();
                void pause();
                void unpause();

                Uint32 get_ticks();
                int time_interval_elapsed();

                void set_interval(int i);

                bool is_started();
                bool is_paused();
        private:
                bool started, paused;
                int interval;
                Uint32 start_ticks, paused_ticks, time_slot;

};
