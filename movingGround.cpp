#include "movingGround.h"

movingGround::movingGround(int curLevel, int Lx, int Rx, int y, int lx, int rx, int ly, int ry, string dire)
{
    //ctor
    mVelX = 0; mVelY = 0;
    lim_lx = lx; lim_rx = rx;
    lim_ly = ly; lim_ry = ry;// cout << ly << ' ' << ry << '\n';
    d = dire;
    mBox.x = Lx; mBox.w = Rx - Lx;
    mBox.y = y; mBox.h = TILE_HEIGHT;
    dir = 1;
    LT = Lx / TILE_WIDTH + (LEVEL_WIDTH[curLevel] / TILE_WIDTH) * (y / TILE_HEIGHT); RT = Rx / TILE_WIDTH - 1 + (LEVEL_WIDTH[curLevel] / TILE_WIDTH) * (y / TILE_HEIGHT);
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
        //cout << LT % (7168 / 32) << ' ' << RT % (7168 / 32) << '\n';
        mVelX = dir * (GROUND_VEL + del);
        int x = tiles[LT]->getBox().x + mVelX; int y = tiles[LT]->getBox().y;
        int x_ = x;
        if(x < lim_lx){
            //tiles[LT]->updateBox(lim_lx, y);
            dir = -dir;
            x_ = lim_lx;
        }
        x = tiles[RT]->getBox().x + mVelX; y = tiles[RT]->getBox().y;
        if(x > lim_rx - TILE_WIDTH){
            tiles[RT]->updateBox(lim_rx - TILE_WIDTH, y);
            dir = -dir;
            x_ = lim_rx - TILE_WIDTH;
        }
        if(x_ != lim_rx - TILE_WIDTH){
            x_ = max(lim_lx, tiles[LT]->getBox().x + mVelX);
            for(int i = LT; i <= RT; i++){
                tiles[i]->updateBox(x_, y); //cout << x_ << ' ';
                x_ += TILE_WIDTH;
            }
            //cout << '\n';
        }
        else{
            //cout << x_ << ' ';
            for(int i = RT - 1; i >= LT; i--){
                tiles[i]->updateBox(x_ - TILE_WIDTH, y);
                //cout << x_ - TILE_WIDTH << ' ';
                x_ -= TILE_WIDTH;
            }
            //cout << '\n';
        }
        mBox.x = tiles[LT]->getBox().x;
        //continue to update (x, y) of each tile in this ground
    }
    else{
        bool ck = 0;
        mVelY = dir * (GROUND_VEL + del);
        if(onmvGround(klee)){
            ck = 1;
        }
        int y = tiles[LT]->getBox().y + mVelY;
        if(y < lim_ly){
            y = lim_ly; dir = -dir;
        }
        if(y + TILE_HEIGHT > lim_ry){
            y = lim_ry - TILE_HEIGHT; dir = -dir;
        }
        for(int i = LT; i <= RT; i++){
            int x = tiles[i]->getBox().x;
            tiles[i]->updateBox(x, y);
        }
        mBox.y = tiles[LT]->getBox().y;
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
