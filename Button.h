#ifndef BUTTON_H
#define BUTTON_H
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include "LTexture.h"

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum ButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture gTButtonSprite;

class Button
{
    public:
        Button();
        void setPosition(int x, int y);
        void handleEvent(SDL_Event* e); //handle mouse evnet
        void render(LTexture gTButtonSprite, SDL_Renderer* gRenderer, SDL_Rect* clip); //show

    protected:

    private:
        SDL_Point mPosition;
        //Global sprite
        ButtonSprite mCurrentSprite;
};

#endif // BUTTON_H
