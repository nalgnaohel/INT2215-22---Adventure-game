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
        exp_val = 30;
    }
    if(type == 1){
        mEnemyVel = 2;
        mEnemyRunVel = 5;
        exp_val = 45;
    }
    frame = 0;
    dir = 1; //1: right, -1: left
    att = 0; run = 0; walk = 1;
    health = 50; mxHealth = 50;
    if(type == 1){
        health += 25; mxHealth += 25;
    }
    ghost_hbar = NULL;
    ghost_hbar = new Healthbar();
    ghost_hbar->setHbVal(mxHealth, health, 100, 50);
    damage = 0;
}

Enemy::~Enemy(){
    int i = 0;
    while(i < (int)mArrowList.size()){
        Arrow* ar = mArrowList.at(i);
        if(ar != NULL){
            mArrowList.erase(mArrowList.begin() + i);
            delete ar;
            ar = NULL;
            i--;
        }
        i++;
    }
    if(ghost_hbar != NULL){
        delete ghost_hbar;
    }
}

bool Enemy::isNearKlee(SDL_Rect& kBox){
    return kBox.y + kBox.h == gr && kBox.x + kBox.w >= mEnemyLeftX && kBox.x <= mEnemyRightX;
}

void Enemy::move(int vel){
    mEnemyBox.x += dir * vel;
    if(mEnemyBox.x < mEnemyLeftX){
        mEnemyBox.x = mEnemyLeftX;
        dir = -dir;
    }
    if(mEnemyBox.x + mEnemyBox.w > mEnemyRightX){
        mEnemyBox.x = mEnemyRightX - mEnemyBox.w; dir = -dir;
    }
}

void Enemy::render(LTexture& gTEnemyHb, LTexture& gTEnemyHbBg, SDL_Rect (&gEnemyHbClips)[2], LTexture& gTEnemy, SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera){
    gTEnemy.render(mEnemyBox.x - camera.x, mEnemyBox.y - camera.y, gRenderer, clip);
    ghost_hbar->setHbVal(mxHealth, health, mEnemyBox.x - camera.x - 10, mEnemyBox.y - camera.y - 20);
    ghost_hbar->render(0, gRenderer, gTEnemyHb, gTEnemyHbBg, gEnemyHbClips);
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

void Enemy::action(Mix_Music* playMusic_fight, Mix_Chunk* stab, Mix_Chunk* ghost_att, SDL_Rect& kBox, SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]){
    if(spriteId % 6 == 1 || (spriteId % 6 == 2 && frame < gEnemySpriteClipsSize[type][2] * GHOST_ID_FRAME[type][2])){
        return;
    }
    if(checkCollision(kBox, mEnemyBox)){
        if(type == 0){
            if(!att){
                spriteId = 0;
                if(dir < 0){
                    spriteId = 6;
                }
                frame = 0; att = 1; walk = 0; run = 0;
                damage = 0;
                updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][spriteId % 6]]);
            }
            else{
                if(frame == 2 * GHOST_ID_FRAME[0][0]){
                    Mix_PlayChannel(-1, stab, 0);
                    damage = 5;
                    //cout << "Knife: " << damage << '\n';
                }
                else{
                    damage = 0;
                }
            }
        }
        else{
            if(!att){
                spriteId = 4;
                if(dir < 0){
                    spriteId = 10;
                }
                frame = 0; damage = 0;
                att = 1; walk = 0; run = 0;
                updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][spriteId % 6]]);
            }
            else{
                if(frame == 2 * GHOST_ID_FRAME[1][4]){
                    Mix_PlayChannel(-1, ghost_att, 0);
                    damage = 5;
                    // << "Scream damage " << damage << '\n';
                }
                else{
                    damage = 0;
                }
            }
        }
    }
    else if(isNearKlee(kBox)){
        if(kBox.x < mEnemyBox.x){
            spriteId = 9; dir = -1;
            if(!run){
                frame = 0; att = 0; run = 1; walk = 0;
            }
            damage = 0;
            move(mEnemyRunVel);
            updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][spriteId % 6]]);
        }
        else{
            spriteId = 3; dir = 1;
            if(!run){
                frame = 0; att = 0; run = 1; walk = 0;
            }
            damage = 0; move(mEnemyRunVel);
            updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][spriteId % 6]]);
        }
    }
    else{
        if(!walk){
            spriteId = 5;
            if(dir < 0){
                spriteId += 6;
            }
            updateEnemyBox(gEnemySpriteClips[type][spriteId][0]);
            frame = 0; att = 0; run = 0; walk = 1;
            damage = 0;
            move(mEnemyVel);
        }
        else{
            spriteId = 5;
            if(dir < 0){
                spriteId += 6;
            }
            move(mEnemyVel);
            damage = 0;
            updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][spriteId % 6]]);
        }
    }
}

void Enemy::updateFrame(Mix_Music* playMusic_fight, Mix_Chunk* stab, Mix_Chunk* ghost_att, SDL_Rect &kBox, SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]){
    //{"Attack_2", "Dead", "Hurt", "Run", "Scream", "Walk"};
    if(spriteId % 6 == 1){
        frame++;
        if(frame >= gEnemySpriteClipsSize[type][1] * GHOST_ID_FRAME[type][1]){
            frame--;
        }
        updateEnemyBox(gEnemySpriteClips[type][spriteId][frame / GHOST_ID_FRAME[type][1]]);
    }
    else if(spriteId % 6 == 2){
        //cout << gEnemySpriteClipsSize[type][2] * GHOST_ID_FRAME[type][2] <<  "    Hurt!Cur frame:          " << frame << '\n';
        frame++;
        if(frame == gEnemySpriteClipsSize[type][2] * GHOST_ID_FRAME[type][2]){
            //cout << "Change!:\n";
            action(playMusic_fight, stab, ghost_att, kBox, gEnemySpriteClips, gEnemySpriteClipsSize);

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

void Enemy::updateHealth(Mix_Chunk* ghost_die[2], int d, SDL_Rect (&gEnemySpriteClips)[2][12][7]){
    health -= d;
    if(health <= 0){
        //die
        if(spriteId % 6 != 1){
            spriteId = 1;
            if(dir < 0) spriteId += 6;
            frame = 0;
            updateEnemyBox(gEnemySpriteClips[type][spriteId][0]);
            Mix_PlayChannel(-1, ghost_die[type], 0);
        }
    }
    else{
        prev_state = spriteId;
        spriteId = 2;
        if(dir < 0) spriteId += 6;
        frame = 0;
        updateEnemyBox(gEnemySpriteClips[type][spriteId][0]);
    }
}

int Enemy::getHealth(){
    return health;
}

int Enemy::getExp_val(){
    return exp_val;
}
