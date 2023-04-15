#include "Timer.h"

Timer::Timer()
{
    //ctor
    tStart = 0; tPause = 0;
    paused = false; started = false;
}

Timer::~Timer()
{
    //dtor

}

void Timer::start(){
    started = true; paused = false;
    tStart = SDL_GetTicks();
}

void Timer::stop(){
    paused = false; started = false;
}

void Timer::pause(){
    if(started == false && paused == false){
        paused = true;
        tPause = SDL_GetTicks() - tStart;
    }
}

void Timer::unpause(){
    if(paused == true){
        paused = false;
        tStart = SDL_GetTicks() - tPause;
        tPause = 0;
    }
}

int Timer::getCurTicks(){
    if(started == true){
        if(paused == true){
            return tPause;
        }
        else{
            return SDL_GetTicks() - tStart;
        }
    }
    return 0;
}

bool Timer::isStarted(){
    return started;
}

bool Timer::isPaused(){
    return paused;
}
