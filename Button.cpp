#include "Button.h"
#include "LTexture.h"

Button::Button()
{
    //ctor
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void Button::setPosition(int x, int y)
{
    mPosition.x = x; mPosition.y = y;
}

void Button::handleEvent(SDL_Event* e)
{
    bool inside = true;
    if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        int x, y; SDL_GetMouseState(&x, &y);
        bool inside = true;
        //check inside
        if(x < mPosition.x){
            inside = false;
        }
        else if(x > mPosition.x + BUTTON_WIDTH){
            inside = false;
        }
        else if(y < mPosition.y){
            inside = false;
        }
        else if(y > mPosition.y + BUTTON_HEIGHT){
            inside = false;
        }
    }
    //if mouse is outside button
    if(!inside){
        mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    }
    else{
        switch(e->type){
            case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION; break;
            case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN; break;
            case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP; break;
        }

    }
}

void Button::render(LTexture gTButtonSprite, SDL_Renderer* gRenderer, SDL_Rect* clip){
    gTButtonSprite.render(mPosition.x, mPosition.y, gRenderer, &gSpriteClips[mCurrentSprite]);
}


