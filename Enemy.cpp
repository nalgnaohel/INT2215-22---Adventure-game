#include "Enemy.h"

Enemy::Enemy(int lx, int y, int rx, int type_, SDL_Rect& box){
    mEnemyBox.x = lx + 3;
    mEnemyLeftX = lx;
    mEnemyRightX = rx;
    gr = y;
    mEnemyBox.w = box.w; mEnemyBox.h = box.h; mEnemyBox.y = gr - box.h;
    spriteId = 5;
    type = type_;
    if(type == 0){
        mEnemyVel = 3;
        mEnemyRunVel = 6;
    }
    if(type == 1){
        mEnemyVel = 2;
        mEnemyRunVel = 5;
    }
    frame = 0;
    dir = 1; //1: right, -1: left
    att = 0;
    health = 50; mxHealth = 50;
    if(type == 1){
        health += 25; mxHealth += 25;
    }
}

Enemy::~Enemy(){
    for(int i = 0; i < (int)mArrowList.size(); i++){
        Arrow* ar = mArrowList.at(i);
        if(ar != NULL){
            mArrowList.erase(mArrowList.begin() + i);
            delete ar;
            ar = NULL;
        }
    }
}

bool Enemy::isNearKlee(SDL_Rect& kBox){
    return kBox.y + kBox.h == gr && kBox.x + kBox.w >= mEnemyLeftX && kBox.x <= mEnemyRightX;
}

void Enemy::move(int vel){
    mEnemyBox.x += dir * vel;
    //cout <<type << ": " << mEnemyBox.x << ' ' << mEnemyLeftX << ' ' << mEnemyRightX << '\n';

    if(mEnemyBox.x < mEnemyLeftX){
        mEnemyBox.x = mEnemyLeftX;
        dir = -dir;
    }
    if(mEnemyBox.x + mEnemyBox.w > mEnemyRightX){
        mEnemyBox.x = mEnemyRightX - mEnemyBox.w; dir = -dir;
    }
}

void Enemy::action(SDL_Rect& kBox, SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]){
    if(type == 0 && spriteId % 6 != 1){
        if(isNearKlee(kBox)){
            //cout << mEnemyBox.x << ' ' << kBox.x << ' ' << abs(mEnemyBox.x - kBox.x) <<'\n';
            int d;
            if(kBox.x < mEnemyBox.x){
                d = mEnemyBox.x - kBox.x - kBox.w;
            }
            else{
                d = kBox.x - mEnemyBox.w - mEnemyBox.x;
            }
            //cout << d << '\n';
            if(d <= 5){
                //attack
                if(!att){
                    spriteId = 0;
                    if(mEnemyBox.x > kBox.x){
                        spriteId += 6;
                    }
                    att = 1; scream = 0; walk = 0; run = 0;
                    frame = 0;
                    updateEnemyBox(gEnemySpriteClips[0][spriteId][0]);
                    damage = 0;
                }
                else{
                    if(checkCollision(kBox, mEnemyBox) && frame == 2 * GHOST_ID_FRAME[0][spriteId % 6]){
                        damage = 5;
                    }
                    else{
                        damage = 0;
                    }
                }
                //cout << "Attack: " << spriteId << '\n';
            }
            else if(d <= 10){
                //scream
                if(!scream){
                    spriteId = 4;
                    if(mEnemyBox.x > kBox.x){
                        spriteId += 6;
                    }
                    //update
                    scream = 1; att = 0; run = 0; walk = 0;
                    frame = 0;
                    updateEnemyBox(gEnemySpriteClips[0][spriteId][0]);
                    damage = 0;
                }
                else{
                    if(frame >= 2 * GHOST_ID_FRAME[0][spriteId % 6] && frame <= 6 * GHOST_ID_FRAME[0][spriteId % 6] && frame % GHOST_ID_FRAME[0][spriteId % 6] == 0){
                        damage = 2;
                    }
                    else{
                        damage = 0;
                    }
                    if(frame == GHOST_ID_FRAME[0][spriteId % 6] * gEnemySpriteClipsSize[0][spriteId] - 1){
                        move(mEnemyRunVel);
                    }
                }
                //cout << "Scream: " << spriteId << '\n';
            }
            else{
                //run
                if(kBox.x < mEnemyBox.x){
                    dir = -1;
                }
                else {
                    dir = 1;
                }
                att = 0; scream = 0; run = 1; walk = 0;
                spriteId = 3;
                if(dir < 0){
                    spriteId += 6;
                }
                //cout << "Run: " << spriteId << '\n';
                frame = 0;
                updateEnemyBox(gEnemySpriteClips[0][spriteId][0]);
                damage = 0;
                move(mEnemyRunVel);

            }
        }
        else{
            if(!walk){
                walk = 1; run = 0; att = 0; scream = 0;
                frame = 0;
                spriteId = 5;
                if(dir < 0){
                    spriteId += 6;
                }
                updateEnemyBox(gEnemySpriteClips[0][spriteId][0]);
            }
            else{
                spriteId = 5;
                if(dir < 0){
                    spriteId += 6;
                }
                updateEnemyBox(gEnemySpriteClips[0][spriteId][0]);
            }
            damage = 0;
            //cout << "Walk: " << spriteId << '\n';
            move(mEnemyVel);
        }
    }
    if(type == 1 && spriteId % 6 != 1){
        if(isNearKlee(kBox)){
            int d;
            if(kBox.x < mEnemyBox.x){
                d = mEnemyBox.x - kBox.x - kBox.w;
            }
            else{
                d = kBox.x - mEnemyBox.w - mEnemyBox.x;
            }
            //cout << d << '\n';
            if(d <= 10){
                //scream
                if(!scream){
                    scream = 1; att = 0; run = 0; walk = 0;
                    spriteId = 4;
                    if(mEnemyBox.x > kBox.x){
                        spriteId += 6;
                    }
                    frame = 0;
                    updateEnemyBox(gEnemySpriteClips[1][spriteId][0]);
                    damage = 0;
                }
                else{
                    if(frame >= 2 * GHOST_ID_FRAME[1][spriteId % 6] && frame < gEnemySpriteClipsSize[1][spriteId] * GHOST_ID_FRAME[1][spriteId % 6] && frame % (GHOST_ID_FRAME[1][spriteId % 6] / 2) == 0){
                        damage = 4;
                    }
                    else{
                        damage = 0;
                    }
                    if(frame == GHOST_ID_FRAME[1][spriteId % 6] * gEnemySpriteClipsSize[1][spriteId] - 1){
                        move(mEnemyRunVel);
                    }
                }
            }
            else{
                //run
                if(kBox.x < mEnemyBox.x){
                    dir = -1;
                }
                else {
                    dir = 1;
                }
                att = 0; scream = 0; run = 1; walk = 0;
                spriteId = 3;
                if(dir < 0){
                    spriteId += 6;
                }
                //cout << "Run: " << spriteId << '\n';
                frame = 0;
                updateEnemyBox(gEnemySpriteClips[1][spriteId][0]);
                damage = 0;
                move(mEnemyRunVel);
            }
        }
        else{
            if(!walk){
                walk = 1; run = 0; att = 0; scream = 0;
                frame = 0;
                spriteId = 5;
                if(dir < 0){
                    spriteId += 6;
                }
                updateEnemyBox(gEnemySpriteClips[1][spriteId][0]);
            }
            else{
                spriteId = 5;
                if(dir < 0){
                    spriteId += 6;
                }
                updateEnemyBox(gEnemySpriteClips[1][spriteId][0]);
            }
            damage = 0;
            //cout << "Walk: " << spriteId << '\n';
            move(mEnemyVel);
        }
    }
}

void Enemy::render(LTexture& gTEnemy, SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera){
    gTEnemy.render(mEnemyBox.x - camera.x, mEnemyBox.y - camera.y, gRenderer, clip);
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

void Enemy::updateEnemyBox(SDL_Rect& box){
    mEnemyBox.w = box.w;
    mEnemyBox.h = box.h;
    mEnemyBox.y = gr - mEnemyBox.h;
}

void Enemy::updateFrame(SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]){
    //{"Attack_2", "Dead", "Hurt", "Run", "Scream", "Walk"};
    if(spriteId % 6 == 1){
        frame++;
        if(frame >= gEnemySpriteClipsSize[type][1] * GHOST_ID_FRAME[type][1]){
            frame--;
        }
        updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][1]]);
    }
    else if(spriteId % 6 == 2){
        frame++;
        if(frame >= gEnemySpriteClipsSize[type][2] * GHOST_ID_FRAME[type][2]){
            spriteId = 5; frame = 0;
        }
        updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][2]]);
    }
    else{
        frame++;
        if(frame >= gEnemySpriteClipsSize[type][spriteId] * GHOST_ID_FRAME[type][spriteId % 6]){
            frame = 0;
        }
        updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][spriteId % 6]]);
    }
}

void Enemy::updateHealth(int d, SDL_Rect (&gEnemySpriteClips)[2][12][7]){
    health -= d;
    if(health <= 0){
        //die
        spriteId = 1;
        if(dir < 0) spriteId += 6;
        frame = 0;
        updateEnemyBox(gEnemySpriteClips[type][spriteId][0]);
    }
    else{
        spriteId = 2;
        if(dir < 0) spriteId += 6;
        frame = 0;
        updateEnemyBox(gEnemySpriteClips[type][spriteId][0]);
    }
}

int Enemy::getHealth(){
    return health;
}
