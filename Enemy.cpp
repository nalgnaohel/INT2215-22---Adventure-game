#include "Enemy.h"

Enemy::Enemy(int lx, int y, int rx){
    mEnemyBox.x = lx + 3; mEnemyBox.y = y;
    mEnemyLeftX = lx;
    mEnemyRightX = rx;
    mEnemyBox.w = 49; mEnemyBox.h = 41;
    mEnemyVel = ENEMY_VEL;
    damage = 5;
    dir = 0; //0: right, 1: left
}

void Enemy::move(){
    if(!dir){
        mEnemyBox.x += mEnemyVel;
    }
    else{
        mEnemyBox.x -= mEnemyVel;
    }
    if(mEnemyBox.x < mEnemyLeftX){
        mEnemyBox.x = mEnemyLeftX; //reach left border
        dir = 0;
    }
    if(mEnemyBox.x > mEnemyRightX){
        mEnemyBox.x = mEnemyRightX; //reach right border
        dir = 1;
    }
}

void Enemy::render(LTexture& gTEnemy, SDL_Renderer* gRenderer, SDL_Rect& camera){
    gTEnemy.render(mEnemyBox.x - camera.x, mEnemyBox.y - camera.y, gRenderer);
}

int Enemy::getEnemyVel(){
    return mEnemyVel;
}

SDL_Rect Enemy::getEnemyBox(){
    return mEnemyBox;
}

int Enemy::getEnemyDamage(){
    return damage;
}

int Enemy::getEnemyLeftX(){
    return mEnemyLeftX;
}

int Enemy::getEnemyRightX(){
    return mEnemyRightX;
}
