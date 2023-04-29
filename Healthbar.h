#ifndef HEALTHBAR_H
#define HEALTHBAR_H
#include "Utils.h"
#include "LTexture.h"

class Healthbar
{
    public:
        Healthbar();
        void setHbVal(int mx_health, int cur_health, int x_, int y_);
        void render(int del, SDL_Renderer* gRenderer, LTexture& gTHealth, LTexture& gTHealthBackground, SDL_Rect (&gHbClips)[2]);

    private:
        int mxHealth; int curHealth;
        int x, y;
};

#endif // HEALTHBAR_H
