#include "Healthbar.h"

Healthbar::Healthbar()
{
    //ctor
}

void Healthbar::setHbVal(int mx_health, int cur_health, int x_, int y_){
    mxHealth = mx_health; curHealth = cur_health;
    x = x_; y = y_;
}

void Healthbar::render(int del, SDL_Renderer* gRenderer, LTexture& gTHealth, LTexture& gTHealthBackground, SDL_Rect (&gHbClips)[2]){
    SDL_Rect clip = gHbClips[1];
    if(curHealth < mxHealth){
        double temp = (double)gHbClips[0].w;
        double tmp = ((double)curHealth / mxHealth) * temp;
        int w_ = int(tmp);
        clip.w = w_;
    }
    gTHealthBackground.render(x, y, gRenderer, &gHbClips[0]);
    gTHealth.render(x + (gHbClips[1].x - gHbClips[0].x), y + del, gRenderer, &clip);
}
