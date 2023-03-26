#include "Klee.h"

Klee::Klee()
{
    //ctor
    mKleePosX = 10; mKleePosY = SCREEN_HEIGHT - 64 - KLEE_HEIGHT;
    mKleeVelX = 0; mKleeVelY = 0;
    mKleeCollider.w = KLEE_WIDTH;
    mKleeCollider.h = KLEE_HEIGHT;
}

void Klee::handleKleeEvent(SDL_Event& e){
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
            case SDLK_UP: mKleeVelY -= KLEE_VEL; break;
            case SDLK_DOWN: mKleeVelY += KLEE_VEL; break;
            case SDLK_LEFT: mKleeVelX -= KLEE_VEL; break;
            case SDLK_RIGHT: mKleeVelX += KLEE_VEL; break;
        }
    }
    else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        switch(e.key.keysym.sym){
            case SDLK_UP: mKleeVelY += KLEE_VEL; break;
            case SDLK_DOWN: mKleeVelY -= KLEE_VEL; break;
            case SDLK_LEFT: mKleeVelX += KLEE_VEL; break;
            case SDLK_RIGHT: mKleeVelX -= KLEE_VEL; break;
        }
    }

}

bool Klee::checkCollision(SDL_Rect a, SDL_Rect b){
    int top_a, top_b;
    int bottom_a, bottom_b;
    int left_a, left_b;
    int right_a, right_b;
    top_a = a.y; top_b = b.y;
    bottom_a = a.y + a.h; bottom_b = b.y + b.h;
    left_a = a.x; left_b = b.x;
    right_a = a.x + a.w; right_b = b.x + b.w;
    if(bottom_a <= top_b){
        return 0;
    }
    if(top_a >= bottom_b){
        return 0;
    }
    if(left_a >= right_b){
        return 0;
    }
    if(right_a <= left_b){
        return 0;
    }
    return 1;
}

void Klee::move(SDL_Rect& object){
    mKleePosX += mKleeVelX; mKleeCollider.x = mKleePosX;
    //check if character moves out of screen
    if((mKleePosX < 0) || (mKleePosX + KLEE_WIDTH > LEVEL_WIDTH) || checkCollision(mKleeCollider, object)){
        mKleePosX -= mKleeVelX; mKleeCollider.x = mKleePosX;
    }
    mKleePosY += mKleeVelY; mKleeCollider.y = mKleePosY;
    if((mKleePosY < 0) || (mKleePosY + KLEE_HEIGHT > LEVEL_HEIGHT) || checkCollision(mKleeCollider, object)){
        mKleePosY -= mKleeVelY; mKleeCollider.y = mKleePosY;
    }
}

void Klee::render(LTexture& gTKleeSpriteSheet, SDL_Renderer* gRenderer, SDL_Rect* clip, int camX, int camY, double angle, SDL_Point* center, SDL_RendererFlip flip){
    gTKleeSpriteSheet.render(mKleePosX - camX, mKleePosY - camY, gRenderer, clip, angle, center, flip);
}

int Klee::getKleePosX(){
    return mKleePosX;
}

int Klee::getKleePosY(){
    return mKleePosY;
}
