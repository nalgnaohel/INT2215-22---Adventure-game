#include "movingGround.h"

movingGround::movingGround(int Lx, int Rx, int y, int lx, int rx, int ly, int ry, string dire)
{
    //ctor
    mVelX = 0; mVelY = 0;
    lim_lx = lx; lim_rx = rx;
    lim_ly = ly; lim_ry = ry;// cout << ly << ' ' << ry << '\n';
    d = dire;
    mBox.x = Lx; mBox.w = Rx - Lx;
    mBox.y = y; mBox.h = TILE_HEIGHT;
    dir = 1;
    LT = Lx / TILE_WIDTH + (LEVEL_WIDTH / TILE_WIDTH) * (y / TILE_HEIGHT); RT = Rx / TILE_WIDTH - 1 + (LEVEL_WIDTH / TILE_WIDTH) * (y / TILE_HEIGHT);
}

bool movingGround::onmvGround(Klee* klee){
    SDL_Rect kleeBox = klee->getKleeBox();
    if(kleeBox.x + kleeBox.w >= mBox.x && kleeBox.x <= mBox.x + mBox.w && kleeBox.y + kleeBox.h == mBox.y){
        return true;
    }
    return false;
}

void movingGround::move(Tile* tiles[], Klee* klee){
    if(d == "lr"){
        mVelX = dir * GROUND_VEL + del;
        int x = tiles[LT]->getBox().x; int y = tiles[LT]->getBox().y;
        int x_ = x;
        if(x < lim_lx){
            tiles[LT]->updateBox(lim_lx, y); dir = -dir;
            x_ = lim_lx;
        }
        x = tiles[RT]->getBox().x; y = tiles[RT]->getBox().y;
        if(x + TILE_WIDTH > lim_rx){
            tiles[RT]->updateBox(lim_rx - TILE_WIDTH, y);
            dir = -dir;
            x_ = lim_rx - TILE_WIDTH;
        }
        if(x_ != lim_rx - TILE_WIDTH){
            x_ = tiles[LT]->getBox().x;
            for(int i = LT + 1; i <= RT; i++){
                tiles[i]->updateBox(x_ + TILE_WIDTH, y);
                x_ += TILE_WIDTH;
            }
        }
        else{
            for(int i = RT - 1; i >= LT; i--){
                tiles[i]->updateBox(x_ - TILE_WIDTH, y);
                x_ -= TILE_WIDTH;
            }
        }
        //continue to update (x, y) of each tile in this ground
    }
    else{
        bool ck = 0;
        mVelY = dir * (GROUND_VEL + del);
        if(onmvGround(klee)){
            ck = 1;
        }
        int tmpY = mBox.y;
        for(int i = LT - 1; i <= RT; i++){
            int x = tiles[i]->getBox().x;
            int y = tiles[i]->getBox().y + mVelY;
            //cout << "Lim tile: "  << lim_ly << ' ' << lim_ry << '\n';
            if(y < lim_ly){
                y = lim_ly; dir = -dir; mVelY = 0;
            }
            if(y + TILE_HEIGHT > lim_ry){
                y = lim_ry - TILE_HEIGHT; dir = -dir; mVelY = 0;
            }
            tiles[i]->updateBox(x, y);
            mBox.y = y;
        }
        //cout << mBox.x << ' ' << mBox.y << '\n';
        if(ck){
            int new_y = mBox.y - klee->getKleeBox().h;
            klee->updatemvGround(new_y);
        }
       // mBox.x = LT * TILE_WIDTH;
    }
}

int movingGround::getLT(){
    return LT;
}

int movingGround::getRT(){
    return RT;
}

int movingGround::getLx(){
    return mBox.x;
}

int movingGround::getRx(){
    return mBox.x + mBox.w;
}

SDL_Rect movingGround::getBox(){
    return mBox;
}

int movingGround::getly(){
    return lim_ly;
}

int movingGround::getry(){
    return lim_ry;
}

void movingGround::setDeltaVel(int delta){
    del = delta;
}
