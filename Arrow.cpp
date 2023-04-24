#include "Arrow.h"

Arrow::Arrow(int a_type)
{
    //ctor
    mBox.x = 0; mBox.y = 0;
    active = 1;
    type = a_type;
    if(type % 3 == 0){
        mVelX = 3; damage = 20;
    }
    if(type % 3 == 1){
        mVelX = 5; damage = 5;
    }
    if(type % 3 == 2){
        mVelX = 4; damage = 6;
    }
    frame = 0;
    dir = 1;
    hit = 0;
}

Arrow::~Arrow()
{
    //dtor
}

void Arrow::move(int x_lim){
    if(active){
        mBox.x += mVelX;// cout << "Bullet coor: " << mBox.x << '\t' << "Bullet vel: " << mVelX << '\n' ;
        if(mBox.x < 0 || mBox.x > x_lim){
            active = 0;
            mVelX = 0;
        }
    }
}

void Arrow::setArrowBox(int x, int y, SDL_Rect (&gArrowSpriteClips)[6][9]){
    mBox.x = x; mBox.y = y;
    mBox.w = gArrowSpriteClips[type][frame / ARROW_FRAME[type % 3]].w;
    mBox.h = gArrowSpriteClips[type][frame / ARROW_FRAME[type % 3]].h;
    //cout << "Arrow rect: " << mBox.x << ' ' << mBox.y << ' ' << mBox.w << ' ' << mBox.h << '\n';
}

int Arrow::getmVelX(){
    return mVelX;
}

bool Arrow::isActive(){
    return active;
}

void Arrow::render(SDL_Renderer* gRenderer, LTexture& gTArrow, SDL_Rect* clip, SDL_Rect& camera){
    gTArrow.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer, clip);
}

SDL_Rect Arrow::getBox(){
    return mBox;
}

void Arrow::setDir(int d){
    dir = d; mVelX *= d;
}

int Arrow::getDamage(){
    return damage;
}

void Arrow::updateFrame(SDL_Rect (&gArrowSpriteClips)[6][9], int (&gArrowSpriteClipsSize)[6]){
    if(type % 3 == 1){
        frame++;
        if(frame == gArrowSpriteClipsSize[type] * ARROW_FRAME[1]){
            frame = 0; active = 0;
        }
        setArrowBox(mBox.x, mBox.y, gArrowSpriteClips);
    }
    if(type % 3 == 0){
        frame++;
        if(!hit){
            frame = min(frame, 5 * ARROW_FRAME[type % 3] - 1);
        }
        if(frame == gArrowSpriteClipsSize[type] * ARROW_FRAME[type % 3]){
            frame = -1; active = 0;
        }
        setArrowBox(mBox.x, mBox.y, gArrowSpriteClips);
    }
    cout << type << ": " << frame << '\n';
}
