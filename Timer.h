#ifndef TIMER_H
#define TIMER_H
#include "Utils.h"

class Timer
{
    public:
        Timer();
        ~Timer();
        void start();
        void stop();
        void pause();
        void unpause();
        int getCurTicks();
        bool isStarted();
        bool isPaused();

    private:
        int tStart; int tPause;
        bool paused; bool started;

};

#endif // TIMER_H
