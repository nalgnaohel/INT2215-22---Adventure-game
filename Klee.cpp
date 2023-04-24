#include "Klee.h"

Klee::Klee(SDL_Rect& box)
{
    //ctor
    mKleeBox.x = 10; mKleeBox.y = SCREEN_HEIGHT - 64 - box.h;
    mKleeVelX = 0; mKleeVelY = 0;
    kleeFrame = 0;
    mLeft = 0; mRight = 0; mJump = 0; mUp = 0; mDown = 0;
    mRun = 0;
    spriteId = 3;
    mKleeBox.w = box.w;
    mKleeBox.h = box.h;
    mKleeMxHealth = 100;
    mKleeHealth = mKleeMxHealth;
    mmvLx = -1; mmvRx = -1; mmvy = -1; st3 = mKleeBox.x;
    fRight = 1; fLeft = 0;
    s_cd = 0;
    klee_hbar = NULL;
    klee_hbar = new Healthbar();
    klee_hbar->setHbVal(mKleeMxHealth, mKleeHealth, 100, 50);
}

Klee::~Klee(){
    for(int i = 0; i < (int)mArrowList.size(); i++){
        Arrow* ar = mArrowList.at(i);
        if(ar != NULL){
            mArrowList.erase(mArrowList.begin() + i);
            delete ar;
            ar = NULL;
        }
    }
    if(klee_hbar != NULL){
        delete klee_hbar; klee_hbar = NULL;
    }
}

void Klee::handleKleeEvent(SDL_Event& e, SDL_Rect (&gKleeSpriteClips)[18][16], vector<pair<pair<int, int>, int>>& ground){
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
        case SDLK_UP:
            {
                if(spriteId != 1) mJump = 1;
            }
        break;
        case SDLK_RIGHT:
            {
                if(mRun){
                    mKleeVelX += KLEE_VEL_SPEED;
                }
                else{
                    mKleeVelX += KLEE_VEL;
                }
                mRight = 1; mLeft = 0;
                fRight = 1; fLeft = 0;
                if(!mJump && !mDown && spriteId != 1){
                    kleeFrame = 0;
                    if(mRun){
                        spriteId = 7; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
                    }
                    else{
                        spriteId = 8, updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
                    }
                }
                if(spriteId == 13){
                    spriteId = 4;
                }
            }
        break;
        case SDLK_LEFT:
            {
                if(mRun){
                    mKleeVelX -= KLEE_VEL_SPEED;
                }
                else{
                    mKleeVelX -= KLEE_VEL;
                }
                mLeft = 1; mRight = 0;
                fLeft = 1; fRight = 0;
                if(!mJump && !mDown && spriteId % 9 != 1){
                    kleeFrame = 0;
                    if(mRun){
                        spriteId = 16; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9] ], ground);
                    }
                    else{
                        spriteId = 17; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9] ], ground);
                    }
                }
                if(spriteId == 4){
                    spriteId += 9;
                }
            }
        break;
        case SDLK_e:
            {
                if(spriteId % 9 != 4 && spriteId % 9!= 1){
                    spriteId = 0; kleeFrame = 0;
                    if(fLeft){
                        spriteId = 9;
                    }
                    updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
                }
            }
        break;
        case SDLK_LSHIFT:
            {
                if(spriteId % 9 != 4 && spriteId %9 != 1){
                    mRun = 1;
                }
            }
        break;
        case SDLK_q:
            {
                if(spriteId % 9 != 4 && spriteId % 9 != 1 && spriteId % 9 != 5){
                    spriteId = 5; kleeFrame = 0;
                    if(fLeft) spriteId += 9;
                    updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground);
                }
            }
        break;
        case SDLK_s:
            {
                if(spriteId % 9 != 4 && spriteId % 9 != 1 && spriteId % 9 != 6 && s_cd == 0){
                    spriteId = 6; kleeFrame = 0;
                    if(fLeft) spriteId += 9;
                    updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground);
                }
            }
        break;
        }

    }
    else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        switch(e.key.keysym.sym){
        case SDLK_UP:
        {
            mJump = 0; mKleeVelY = 0;
        }
        break;
        case SDLK_LEFT:
            {
                mLeft = 0; mKleeVelX = 0;
                if(spriteId == 8 || spriteId == 7){
                    spriteId = 3;
                }
                if(spriteId == 17 || spriteId == 16){
                    spriteId = 12;
                }
                kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
            }
        break;
        case SDLK_RIGHT:
            {
                mRight = 0; mKleeVelX = 0;
                if(spriteId == 8 || spriteId == 7){
                    spriteId = 3;
                }
                if(spriteId == 17 || spriteId == 16){
                    spriteId = 12;
                }
                kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
            }
        break;
        case SDLK_e:
            {
                if(spriteId == 0){
                    spriteId = 3;
                }
                if(spriteId == 9){
                    spriteId = 12;
                }
                kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
            }
        break;
        case SDLK_LSHIFT:
            {
                mRun = 0;
            }
        break;
        }

    }
}

void Klee::reset(SDL_Rect& box){
    mKleeBox.x = 10; mKleeBox.y = SCREEN_HEIGHT - box.h - 2 * TILE_HEIGHT;
    mKleeVelX = 0; mKleeVelY = 0;
    kleeFrame = 0;
    mLeft = 0; mRight = 0; mJump = 0; mUp = 0; mDown = 0;
    spriteId = 3;
    mKleeBox.w = box.w;
    mKleeBox.h = box.h;
    mKleeMxHealth = 100;
    mKleeHealth = mKleeMxHealth;
    mmvLx = -1; mmvRx = -1; mmvy = -1; st3 = 10;
    fLeft = 0; fRight = 1;
}

bool Klee::onGround(vector<pair<pair<int, int>, int>>& ground){
    for(int i = 0; i < (int)ground.size(); i++){
        //cout << ground[i].fi.fi * TILE_WIDTH << ' ' << ground[i].fi.se * TILE_WIDTH << ' ' << ground[i].se * TILE_HEIGHT << '\n';
        int lx_ground = ground[i].fi.fi;
        int rx_ground = ground[i].fi.se + TILE_WIDTH;
        int y_ground = ground[i].se;
        //cout << mKleeBox
        if(mKleeBox.y + mKleeBox.h == y_ground){
            if(lx_ground <= mKleeBox.x + mKleeBox.w && mKleeBox.x <= rx_ground){
                //cout << "Ground: " << mKleeBox.y + mKleeBox.h << " - " << lx_ground << ' ' << rx_ground << ' ' << y_ground << '\n';
                return 1;
            }
        }
    }
    return 0;
}

bool Klee::touchObjects(Tile* tiles[]){
    for(int i = 0; i < TOTAL_TILES; i++){
        int ttype = tiles[i]->getType(); //cout << ttype << ' ';
        bool obj = false;
        for(int j = 0; j < 10; j++){
            if(ttype == OBJECTS_ID[j]){
                //cout << tiles[i]->getBox().x << ' ' << tiles[i]->getBox().y << '\n';
                 obj = true; break;
            }
        }
        if(obj){
            bool res = true;
            SDL_Rect a = mKleeBox;
            SDL_Rect b = tiles[i]->getBox();
            int top_a, top_b;
            int bottom_a, bottom_b;
            int left_a, left_b;
            int right_a, right_b;
            top_a = a.y; top_b = b.y;
            bottom_a = a.y + a.h; bottom_b = b.y + b.h;
            left_a = a.x; left_b = b.x;
            right_a = a.x + a.w; right_b = b.x + b.w;
            if(bottom_a <= top_b){
                res = false;
            }
            if(top_a > bottom_b){
                res = false;
            }
            if(left_a > right_b){
                res = false;
            }
            if(right_a < left_b){
                res = false;
            }
            if(res){
                return true;
            }
        }
    }
    return false;
}

void Klee::move(Tile* tiles[], vector<pair<pair<int, int>, int>>& ground, SDL_Rect (&gKleeSpriteClips)[18][16]){
    if(mJump && !mDown){
        if(onGround(ground)){
            spriteId = 4; kleeFrame = 0;
            if(fLeft) spriteId += 9;
            updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
            mnHeight = mKleeBox.y - 8 * TILE_HEIGHT - 16;
        }
        mKleeVelY += KLEE_VEL;
        int py = mKleeBox.y; mKleeBox.y -= mKleeVelY;
        if(mKleeBox.y < max(0, mnHeight)){
            mKleeBox.y = max(0, mnHeight);
            mJump = 0; mKleeVelY = 0;
        }
        for(int i = 0; i < (int)ground.size(); i++){
            int lx_ground = ground[i].fi.fi;
            int rx_ground = ground[i].fi.se + TILE_WIDTH;
            int y_ground = ground[i].se;
            if(lx_ground <= mKleeBox.x + mKleeBox.w && mKleeBox.x <= rx_ground){
                if(mKleeBox.y <= y_ground + TILE_HEIGHT && py >= y_ground + TILE_HEIGHT){
                    //cout << "Ground: " << mKleeBox.y << ' ' << py << " - " << lx_ground << ' ' << rx_ground << ' ' << y_ground + TILE_HEIGHT << '\n';
                    mKleeBox.y = y_ground + TILE_HEIGHT;
                    mJump = 0; mKleeVelY = 0;
                    break;
                }
            }
        }
        //cout << "After jump: " << mKleeBox.y << '\n';
    }
    if(mJump == 0 && !onGround(ground)){
        mDown = 1;
    }
    if(mDown){
        spriteId = 4;
        if(fLeft) spriteId += 9;
        if(kleeFrame < 5 * KLEE_ID_FRAME[spriteId % 9]){
            kleeFrame = 5 * KLEE_ID_FRAME[spriteId % 9]; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
        }
        else{
            kleeFrame++;
            if(kleeFrame == 6 * KLEE_ID_FRAME[spriteId % 9]){
                kleeFrame--;
            }
            updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground);
        }
        mKleeVelY += GRAVITY_SPEED;
        if(mKleeVelY > MAX_FALL_SPEED){
            mKleeVelY = MAX_FALL_SPEED;
        }
        int pdy = mKleeBox.y;
        mKleeBox.y += mKleeVelY;
        for(int i = 0; i < (int)ground.size(); i++){
            if(mKleeBox.x + mKleeBox.w >= ground[i].fi.fi && mKleeBox.x <= ground[i].fi.se + TILE_WIDTH){
                if(mKleeBox.y + mKleeBox.h >= ground[i].se && pdy <= ground[i].se){
                    mKleeVelY = 0; mDown = 0;
                    //cout << "Expected to touch here: " << ground[i].fi.fi * TILE_WIDTH << ' ' << ground[i].fi.se * TILE_WIDTH << ' ' << ground[i].se * TILE_HEIGHT << '\n';
                    kleeFrame = 6 * KLEE_ID_FRAME[spriteId % 9];
                    SDL_Rect newBox = {817, 70, 31, 58};
                    updateKleeBox(newBox, ground);
                    mKleeBox.y = ground[i].se - mKleeBox.h;
                    break;
                }
            }
        }
    }

    mKleeBox.x += mKleeVelX;
    if(mKleeBox.x < 0 || mKleeBox.x + mKleeBox.w > LEVEL_WIDTH || touchObjects(tiles)){
        mKleeBox.x -= mKleeVelX;
    }

}

void Klee::render(LTexture& gTKleeSpriteSheet, LTexture& gTKleeHb, LTexture& gTKleeHbBackground, SDL_Rect (&gKleeHbClips)[2], SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera){
    gTKleeSpriteSheet.render(mKleeBox.x - camera.x, mKleeBox.y - camera.y, gRenderer, clip);
    klee_hbar->setHbVal(mKleeMxHealth, mKleeHealth, mKleeBox.x - camera.x - 10, mKleeBox.y - camera.y - 20);
    klee_hbar->render(gRenderer, gTKleeHb, gTKleeHbBackground,gKleeHbClips);
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

void Klee::updateKleeBox(SDL_Rect& box, vector<pair<pair<int, int>, int>>& ground){
    bool ck = onGround(ground); int f = mKleeBox.y + mKleeBox.h;
    mKleeBox.w = box.w;
    mKleeBox.h = box.h;
    if(ck){
        mKleeBox.y = f - mKleeBox.h;
    }
}

void Klee::updatemvGround(int& y){
    mKleeBox.y = y;
}

int Klee::getKleeHealth(){
    return mKleeHealth;
}

void Klee::updateKleeHealth(int damage){
    mKleeHealth -= damage;
}

bool Klee::isJump(){
    return mJump;
}

bool Klee::isDown(){
    return mDown;
}

void Klee::handleArrowList(SDL_Renderer* gRenderer, LTexture (&gTArrow)[6], SDL_Rect (&gArrowSpriteClips)[6][9], int (&gArrowSpriteClipsSize)[6], SDL_Rect& camera, vector<Enemy*> (&ghost)[TOTAL_ENEMIES], SDL_Rect (&gEnemySpriteClips)[2][12][7]){
    for(int i = 0; i < (int)mArrowList.size(); i++){
        Arrow* ar = mArrowList.at(i);
        if(ar != NULL){
            if(ar->isActive()){
                if(!ar->hit) ar->move(camera.x + SCREEN_WIDTH);
                for(int k = 0; k < TOTAL_ENEMIES; k++){
                    for(int j = 0; j < (int)ghost[k].size(); j++){
                        SDL_Rect eBox = ghost[k][j]->getEnemyBox();
                        SDL_Rect arBox = ar->getBox();
                        if(checkCollision(arBox, eBox)){
                            if(!ar->hit){
                                ghost[k][j]->updateHealth(ar->getDamage(), gEnemySpriteClips);
                                cout << "Hit! -  Health: " << ghost[k][j]->getHealth() << '\n';
                                ar->hit = 1;
                                if(ar->type % 3 == 0){
                                    ar->frame = 5 * ARROW_FRAME[0];
                                }
                            }
                        }
                    }
                }
                SDL_Rect* clip = &gArrowSpriteClips[ar->type][ar->frame / ARROW_FRAME[ar->type % 3]];
                ar->render(gRenderer, gTArrow[ar->type], clip, camera);
                ar->updateFrame(gArrowSpriteClips, gArrowSpriteClipsSize);
            }
            else{
                if(ar->type % 3 == 0 && ar->frame == -1){
                    s_cd = 20;
                }
                mArrowList.erase(mArrowList.begin() + i);
                delete ar;
                ar = NULL;
            }
        }
    }
}

void Klee::updateFrame(SDL_Rect (&gKleeSpriteClips)[18][16], int (&gKleeSpriteClipsSize)[18], vector<pair<pair<int, int>, int>>& ground, SDL_Rect (&gArrowSpriteClips)[6][9], SDL_Rect& camera){
    //{"Attack_1", "Dead", "Hurt", "Idle", "Jump", "Magic_arrow", "Magic_sphere", "Run", "Walk"};
    if(spriteId % 9 != 4 && spriteId % 9 != 0){
        if(spriteId % 9 == 3){
            kleeFrame = 0;
        }
        else if(spriteId % 9 == 2){
            kleeFrame++;
            if(kleeFrame >= KLEE_ID_FRAME[spriteId % 9] * gKleeSpriteClipsSize[spriteId]){
                spriteId = 3; kleeFrame = 0;
            }
        }
        else if(spriteId % 9 == 5){
            kleeFrame++;
            if(kleeFrame == KLEE_ID_FRAME[spriteId % 9] * 5){
                int s_type = 1;
                if(fLeft) s_type += 3;
                Arrow* arrow = new Arrow(s_type);
                if(s_type < 3){
                    arrow->setArrowBox(mKleeBox.x + mKleeBox.w, mKleeBox.y + mKleeBox.h / 2 - 10, gArrowSpriteClips);
                }
                else{
                    arrow->setArrowBox(mKleeBox.x, mKleeBox.y + mKleeBox.h / 2 - 10, gArrowSpriteClips);
                }
                if(fLeft){
                    arrow->setDir(-1);
                }
                else{
                    arrow->setDir(1);
                }
                mArrowList.pb(arrow);
            }
            if(kleeFrame >= KLEE_ID_FRAME[spriteId % 9] * gKleeSpriteClipsSize[spriteId]){
                spriteId -= 2; kleeFrame = 0;
            }
        }
        else if(spriteId % 9 == 6){
            kleeFrame++;
            if(kleeFrame == KLEE_ID_FRAME[spriteId % 9] * 14){
                int s_type = 0;
                if(fLeft) s_type += 3;
                Arrow* arrow = new Arrow(s_type);
                if(s_type < 3){
                    arrow->setArrowBox(mKleeBox.x + mKleeBox.w, mKleeBox.y + mKleeBox.h / 3, gArrowSpriteClips);
                }
                else{
                    arrow->setArrowBox(mKleeBox.x, mKleeBox.y + mKleeBox.h / 3, gArrowSpriteClips);
                }
                if(fLeft){
                    arrow->setDir(-1);
                }
                else{
                    arrow->setDir(1);
                }
                mArrowList.pb(arrow);
            }
            if(kleeFrame >= KLEE_ID_FRAME[spriteId % 9] * gKleeSpriteClipsSize[spriteId]){
                spriteId -= 3; kleeFrame = 0;
            }
        }
        else{
            kleeFrame++;
            if(kleeFrame / KLEE_ID_FRAME[spriteId % 9] >= gKleeSpriteClipsSize[spriteId]){
                kleeFrame = 0;
            }
        }
        updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground);
    }
    else if(spriteId % 9 == 4) {
        if(kleeFrame >= 6 * KLEE_ID_FRAME[spriteId % 9]){
            kleeFrame++;
            if(kleeFrame == 8 * KLEE_ID_FRAME[spriteId % 9]){
                spriteId = 3 + 9 * (spriteId / 9); kleeFrame = 0;
            }
        }
        updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground);
    }
    else{
        kleeFrame++;
        if(kleeFrame / KLEE_ID_FRAME[spriteId % 9] >= gKleeSpriteClipsSize[0]){
            kleeFrame = 0;
        }
        updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground);
    }
}
