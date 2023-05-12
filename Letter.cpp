#include "Letter.h"

Letter::Letter(int x_, SDL_Rect& box)
{
    //ctor
    mBox.x = x_; mBox.y = SCREEN_HEIGHT - 2 * TILE_HEIGHT - box.h;
    mBox.w = box.w; mBox.h = box.h;
    dir = -1; mLetterVel = 1;
}

Letter::~Letter()
{
    //dtor
}

void Letter::move(){
    mBox.y += dir * mLetterVel;
    if(mBox.y < SCREEN_HEIGHT - 2 * TILE_HEIGHT - mBox.h - 40){
        mBox.y = SCREEN_HEIGHT - 2 * TILE_HEIGHT - mBox.h - 40; dir = -dir;
    }
    if(mBox.y + mBox.h > SCREEN_HEIGHT - 2 * TILE_HEIGHT){
        dir = -dir; mBox.y = SCREEN_HEIGHT - 2 * TILE_HEIGHT - mBox.h;
    }
}

void Letter::render(SDL_Renderer* gRenderer, LTexture& gTLetter, SDL_Rect* gLetter){
    gTLetter.render(mBox.x, mBox.y, gRenderer, gLetter);
}

SDL_Rect Letter::getBox(){
    return mBox;
}
