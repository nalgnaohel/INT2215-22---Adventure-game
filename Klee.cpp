#include "Klee.h"

Klee::Klee()
{
    //ctor
    mKleeBox.x = 10; mKleeBox.y = SCREEN_HEIGHT - 64 - KLEE_HEIGHT;
    mKleeVelX = 0; mKleeVelY = 0;
    mKleeBox.w = KLEE_WIDTH;
    mKleeBox.h = KLEE_HEIGHT;
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

bool Klee::touchObjects(Tile* tiles[]){
    bool res = false;
    for(int i = 0; i < TOTAL_TILES; i++){
        //check wall type tile and collision
        int ttype = tiles[i]->getType(); //cout << ttype << ' ';
        bool obj = false;
        for(int j = 0; j < 10; j++){
            if(ttype == OBJECTS_ID[j]){
                //cout << tiles[i]->getBox().x << ' ' << tiles[i]->getBox().y << '\n';
                 obj = true; break;
            }
        }
        SDL_Rect tileBox = tiles[i]->getBox(); tileBox.y += 2;
        if(obj && checkCollision(mKleeBox, tileBox)){
            res = true; break;
        }
    }
    return res;
}

void Klee::move(Tile* tiles[]){
    mKleeBox.x += mKleeVelX;
    //check if character moves out of screen or touch other objects
    if((mKleeBox.x < 0) || (mKleeBox.x + KLEE_WIDTH > LEVEL_WIDTH) || touchObjects(tiles)){
        mKleeBox.x -= mKleeVelX;
    }
    mKleeBox.y += mKleeVelY;
    if((mKleeBox.y < 0) || (mKleeBox.y + KLEE_HEIGHT > LEVEL_HEIGHT)|| touchObjects(tiles)){
        mKleeBox.y -= mKleeVelY;
    }
}

void Klee::render(LTexture& gTKleeSpriteSheet, SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera){
    gTKleeSpriteSheet.render(mKleeBox.x - camera.x, mKleeBox.y - camera.y, gRenderer, clip);
}

void Klee::setCamera(SDL_Rect& camera){
    camera.x = mKleeBox.x - 100;
    camera.y = 0;

    //Keep the camera in bounds
    if(camera.x < 0){
        camera.x = 0;
    }
    if(camera.y < 0){
        camera.y = 0;
    }
    if(camera.x > LEVEL_WIDTH - camera.w){
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if(camera.y > LEVEL_HEIGHT - camera.h){
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

int Klee::getKleeVelX(){
    return mKleeVelX;
}

int Klee::getKleeVelY(){
    return mKleeVelY;
}

SDL_Rect Klee::getKleeBox(){
    return mKleeBox;
}
