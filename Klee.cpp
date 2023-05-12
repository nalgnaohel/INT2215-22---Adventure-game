#include "Klee.h"

Klee::Klee(SDL_Rect& box)
{
    //ctor
    mKleeBox.x = 10; mKleeBox.y = SCREEN_HEIGHT - 64 - box.h;
    mKleeVelX = 0; mKleeVelY = 0;
    kleeFrame = 0;
    mLeft = 0; mRight = 0; mJump = 0; mUp = 0; mDown = 0;
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
    exp = 0;
    klee_exp_bar = NULL; klee_exp_bar = new Healthbar();
    klee_exp_bar->setHbVal(EXP_LIM, exp, 150, 100);
    lim_cd = 20 * FRAME_PER_SECOND;
    feet.fi = 0; feet.se = 0;
    level = 0;
    ins = 0;
    last_ground.fi = mKleeBox.x; last_ground.se = mKleeBox.y;
    life = 3;
}

Klee::~Klee(){
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
    if(klee_hbar != NULL){
        delete klee_hbar; klee_hbar = NULL;
    }
    if(klee_exp_bar != NULL){
        delete klee_exp_bar; klee_exp_bar = NULL;
    }
}

void Klee::handleKleeEvent(SDL_Event& e, SDL_Rect (&gKleeSpriteClips)[18][16], vector<pair<pair<int, int>, int>>& ground, pair<int, int> (&kFeet)[9][16]){
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
        case SDLK_UP:
            {
                if(spriteId != 1) mJump = 1;
            }
        break;
        case SDLK_RIGHT:
            {
                mKleeVelX += KLEE_VEL;
                mRight = 1; mLeft = 0;
                fRight = 1; fLeft = 0;
                if((spriteId % 9 != 4 || (spriteId % 9 == 4 && kleeFrame >= 6 * KLEE_ID_FRAME[4])) && spriteId % 9 != 1){
                    kleeFrame = 0; //cout << "Change to 0\n";
                    spriteId = 8, updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);

                }
                if(spriteId == 13){
                    spriteId = 4;
                }
            }
        break;
        case SDLK_LEFT:
            {
                mKleeVelX -= KLEE_VEL;
                mLeft = 1; mRight = 0;
                fLeft = 1; fRight = 0;
                if((spriteId % 9 != 4 || (spriteId % 9 == 4 && kleeFrame >= 6 * KLEE_ID_FRAME[4])) && spriteId % 9 != 1){
                    kleeFrame = 0;
                    spriteId = 17; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9] ], ground, kFeet);
                }
                if(spriteId == 4){
                    spriteId += 9;
                }
            }
        break;
        case SDLK_e:
            {
                if(spriteId % 9 != 4 && spriteId % 9!= 1 && spriteId % 9 != 0){
                    spriteId = 0; kleeFrame = 0;
                    if(fLeft){
                        spriteId = 9;
                    }
                    updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
                }
            }
        break;
        case SDLK_q:
            {
                if(spriteId % 9 != 4 && spriteId % 9 != 1 && spriteId % 9 != 5){
                    spriteId = 5; kleeFrame = 0;
                    if(fLeft) spriteId += 9;
                    updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
                }
            }
        break;
        case SDLK_s:
            {
                if(spriteId % 9 != 4 && spriteId % 9 != 1 && spriteId % 9 != 6 && s_cd == 0){
                    spriteId = 6; kleeFrame = 0;
                    if(fLeft) spriteId += 9;
                    updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
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
                    spriteId = 3; kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
                }
                if(spriteId == 17 || spriteId == 16){
                    spriteId = 12; kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
                }

            }
        break;
        case SDLK_RIGHT:
            {
                mRight = 0; mKleeVelX = 0;
                if(spriteId == 8 || spriteId == 7){
                    spriteId = 3; kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
                }
                if(spriteId == 17 || spriteId == 16){
                    spriteId = 12; kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
                }

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
                kleeFrame = 0; updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
            }
        break;
        }

    }
}

bool Klee::onGround(vector<pair<pair<int, int>, int>>& ground){
    for(int i = 0; i < (int)ground.size(); i++){
        //cout << ground[i].fi.fi * TILE_WIDTH << ' ' << ground[i].fi.se * TILE_WIDTH << ' ' << ground[i].se * TILE_HEIGHT << '\n';
        int lx_ground = ground[i].fi.fi;
        int rx_ground = ground[i].fi.se + TILE_WIDTH;
        int y_ground = ground[i].se;
        //cout << mKleeBox
        if(mKleeBox.y + mKleeBox.h == y_ground){
            int dlx = feet.fi;
            int drx = feet.se;
            if(spriteId >= 9){
                swap(dlx, drx);
            }
            if(lx_ground <= mKleeBox.x + mKleeBox.w - drx && mKleeBox.x + dlx <= rx_ground){
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

void Klee::move(int curLevel, map<pair<pair<int, int>, int>, bool>& is_mv, Tile* tiles[], vector<pair<pair<int, int>, int>>& ground, SDL_Rect (&gKleeSpriteClips)[18][16], pair<int, int> (&kFeet)[9][16]){
    if(mJump && !mDown){
        if(onGround(ground)){
            spriteId = 4; kleeFrame = 0;
            if(fLeft) spriteId += 9;
            //cout << "Jump\n";
            updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
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
            //cout << lx_ground << ' ' << rx_ground << ' ' << y_ground << '\n';
            int lf = feet.fi; int rf = feet.se;
            if(spriteId >= 9){
                swap(lf, rf);
            }
            if(lx_ground <= mKleeBox.x + mKleeBox.w - rf && mKleeBox.x <= rx_ground + lf){
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
        kleeFrame = 5 * KLEE_ID_FRAME[spriteId % 9];
        updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame/KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
        mKleeVelY += GRAVITY_SPEED;
        if(mKleeVelY > MAX_FALL_SPEED){
            mKleeVelY = MAX_FALL_SPEED;
        }
        int pdy = mKleeBox.y;
        mKleeBox.y += mKleeVelY;
        for(int i = 0; i < (int)ground.size(); i++){
            //cout << ground[i].fi.fi <<  ' ' << ground[i].fi.se << ' ' << ground[i].se << '\n';
            int lf = feet.fi; int rf = feet.se;
            if(spriteId >= 9){
                swap(lf, rf);
            }
            if(mKleeBox.x + mKleeBox.w - rf >= ground[i].fi.fi && mKleeBox.x + lf <= ground[i].fi.se + TILE_WIDTH){
                if(mKleeBox.y + mKleeBox.h >= ground[i].se && pdy <= ground[i].se){
                    mKleeVelY = 0; mDown = 0;
                    //cout << "Expected to touch here: " << ground[i].fi.fi << ' ' << ground[i].fi.se << ' ' << ground[i].se << '\n';
                    kleeFrame = 6 * KLEE_ID_FRAME[spriteId % 9];
                    SDL_Rect newBox = {817, 70, 31, 58};
                    updateKleeBox(newBox, ground, kFeet);
                    mKleeBox.y = ground[i].se - mKleeBox.h;
                    break;
                }
            }
        }
    }
    //if(spriteId % 9 != 4 || kleeFrame < 6 * KLEE_ID_FRAME[4]){
    mKleeBox.x += mKleeVelX;
    if(mKleeBox.x < 0 || mKleeBox.x + mKleeBox.w > LEVEL_WIDTH[curLevel] || touchObjects(tiles)){
        mKleeBox.x -= mKleeVelX;
    }
    //}

    for(int i = 0; i < (int)ground.size(); i++){
        //cout << ground[i].fi.fi << ' ' << ground[i].fi.se << ' ' << ground[i].se << ' ' << is_mv[ground[i]] << '\n';
        int lx_ground = ground[i].fi.fi;
        int rx_ground = ground[i].fi.se + TILE_WIDTH;
        int y_ground = ground[i].se;
        //cout << mKleeBox
        if(mKleeBox.y + mKleeBox.h == y_ground){
            int dlx = feet.fi;
            int drx = feet.se;
            if(spriteId >= 9){
                swap(dlx, drx);
            }
            if(lx_ground <= mKleeBox.x + mKleeBox.w - drx && mKleeBox.x + dlx <= rx_ground && is_mv[ground[i]] == 0){
                last_ground.fi = lx_ground + 20; last_ground.se = y_ground - mKleeBox.h;
            }
        }
    }

}

void Klee::render(LTexture& gTKleeSpriteSheet, LTexture& gTKleeHb, LTexture& gTKleeHbBackground, SDL_Rect (&gKleeHbClips)[2], LTexture& gTKleeExp, LTexture& gTKleeExpBg, SDL_Rect (&gKleeExpbClips)[2], SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera){
    gTKleeSpriteSheet.render(mKleeBox.x - camera.x, mKleeBox.y - camera.y, gRenderer, clip);
    klee_hbar->setHbVal(mKleeMxHealth, mKleeHealth, mKleeBox.x - camera.x - 10, mKleeBox.y - camera.y - 20);
    klee_hbar->render(6, gRenderer, gTKleeHb, gTKleeHbBackground,gKleeHbClips);
    klee_exp_bar->setHbVal(EXP_LIM, exp, SCREEN_WIDTH  / 2 + 50, 10);
    klee_exp_bar->render(0, gRenderer, gTKleeExp, gTKleeExpBg, gKleeExpbClips);
}

void Klee::setCamera(int curLevel, SDL_Rect& camera){
    camera.x = mKleeBox.x - 100;
    camera.y = 0;

    //Keep the camera in bounds
    if(camera.x < 0){
        camera.x = 0;
    }
    if(camera.y < 0){
        camera.y = 0;
    }
    if(camera.x > LEVEL_WIDTH[curLevel] - camera.w){
        camera.x = LEVEL_WIDTH[curLevel] - camera.w;
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

void Klee::updateKleeBox(SDL_Rect& box, vector<pair<pair<int, int>, int>>& ground, pair<int, int> (&kFeet)[9][16]){
    bool ck = 0; int gr_in;
    for(int i = 0; i < (int)ground.size(); i++){
        int lx_ground = ground[i].fi.fi;
        int rx_ground = ground[i].fi.se + TILE_WIDTH;
        int y_ground = ground[i].se;

        if(mKleeBox.y + mKleeBox.h == y_ground){
            int dlx = feet.fi;
            int drx = feet.se;
            if(spriteId >= 9){
                swap(dlx, drx);
            }
            if(lx_ground <= mKleeBox.x + mKleeBox.w - drx && mKleeBox.x + dlx <= rx_ground){
                gr_in = i;
                ck = 1; break;
            }
        }
    }
    int f = mKleeBox.y + mKleeBox.h;
    mKleeBox.w = box.w;
    mKleeBox.h = box.h;
    if(ck){
        mKleeBox.y = f - mKleeBox.h;
        if(spriteId % 9 != 8 && spriteId % 9 != 4 && spriteId % 9 != 3){
            int new_dellfeet = kFeet[spriteId % 9][kleeFrame / KLEE_ID_FRAME[spriteId % 9]].fi;
            int new_delrfeet = kFeet[spriteId % 9][kleeFrame / KLEE_ID_FRAME[spriteId % 9]].se;
            if(spriteId >= 9){
                swap(new_dellfeet, new_delrfeet);
            }

            if(mKleeBox.x + mKleeBox.w - new_delrfeet < ground[gr_in].fi.fi){
                //cout << "Hold tight!\n";
                mKleeBox.x = ground[gr_in].fi.fi + new_delrfeet - mKleeBox.w;
            }
            if(mKleeBox.x + new_delrfeet > ground[gr_in].fi.se){
                //cout << "Hold tight!\n";
                mKleeBox.x = ground[gr_in].fi.se - new_delrfeet;
            }
        }
    }
    feet = kFeet[spriteId % 9][kleeFrame / KLEE_ID_FRAME[spriteId % 9]];
}

void Klee::updatemvGround(int& x, int& y){
    mKleeBox.x = x;
    mKleeBox.y = y;
}

int Klee::getKleeHealth(){
    return mKleeHealth;
}

void Klee::updateKleeHealth(Mix_Chunk* pState, int d, SDL_Rect (&gKleeSpriteClips)[18][16]){
    if(spriteId % 9 != 6 && spriteId % 9 != 1){
        mKleeHealth -= d;
        if(d > 0){
            if(spriteId % 9 != 2){
                spriteId = 2;
                if(fLeft){
                    spriteId += 9;
                }
                kleeFrame = 0;
                Mix_PlayChannel(-1, pState, 0);
            }
        }
    }
}

bool Klee::isJump(){
    return mJump;
}

bool Klee::isDown(){
    return mDown;
}

void Klee::handleArrowList(Mix_Chunk* sound_q, Mix_Chunk* sound_s, Mix_Chunk* ghost_die[2], SDL_Renderer* gRenderer, LTexture (&gTArrow)[6], SDL_Rect (&gArrowSpriteClips)[6][9], int (&gArrowSpriteClipsSize)[6], SDL_Rect& camera, vector<Enemy*> (&ghost)[TOTAL_ENEMIES], SDL_Rect (&gEnemySpriteClips)[2][12][7]){
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
                                if(ar->type % 3 == 0){
                                    Mix_PlayChannel(-1, sound_s, 0);
                                }
                                if(ar->type % 3 == 1){
                                    Mix_PlayChannel(-1, sound_q, 0);
                                }
                                ghost[k][j]->updateHealth(ghost_die, ar->getDamage(), gEnemySpriteClips);
                                //cout << "Hit! -  Health: " << ghost[k][j]->getHealth() << '\n';
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
                    s_cd = lim_cd;
                }
                mArrowList.erase(mArrowList.begin() + i);
                delete ar;
                ar = NULL;
            }
        }
    }
}

void Klee::updateFrame(SDL_Rect (&gKleeSpriteClips)[18][16], int (&gKleeSpriteClipsSize)[18], vector<pair<pair<int, int>, int>>& ground, SDL_Rect (&gArrowSpriteClips)[6][9], SDL_Rect& camera, pair<int, int> (&kFeet)[9][16]){
    //{"Attack_1", "Dead", "Hurt", "Idle", "Jump", "Magic_arrow", "Magic_sphere", "Run", "Walk"};
    if(spriteId % 9 != 4 && spriteId % 9 != 0){
        if(spriteId % 9 == 3){
            kleeFrame = 0;
        }
        else if(spriteId % 9 == 2){
            kleeFrame++;
            if(kleeFrame >= KLEE_ID_FRAME[spriteId % 9] * gKleeSpriteClipsSize[spriteId]){
                spriteId += 1; kleeFrame = 0;
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
        updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
    }
    else if(spriteId % 9 == 4) {
        //cout << kleeFrame << ' ' << kleeFrame / KLEE_ID_FRAME[4] << '\n';
        if(kleeFrame < 5 * KLEE_ID_FRAME[spriteId % 9]){
            kleeFrame++;
            if(kleeFrame == 5 * KLEE_ID_FRAME[spriteId % 9]){
                kleeFrame--;
            }
        }
        if(kleeFrame >= 6 * KLEE_ID_FRAME[spriteId % 9]){
            kleeFrame++;
            if(kleeFrame == 8 * KLEE_ID_FRAME[spriteId % 9]){
                spriteId = 3 + 9 * (spriteId / 9); kleeFrame = 0;
            }
        }
        updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
    }
    else{
        kleeFrame++;
        if(kleeFrame / KLEE_ID_FRAME[spriteId % 9] >= gKleeSpriteClipsSize[0]){
            kleeFrame = 0;
        }
        updateKleeBox(gKleeSpriteClips[spriteId][kleeFrame / KLEE_ID_FRAME[spriteId % 9]], ground, kFeet);
    }
}

void Klee::updateExp(Mix_Chunk* lv_up, int val){
    exp += val;
    if(exp >= EXP_LIM){
        Mix_PlayChannel(-1, lv_up, 0);
        level++;
        if(level % 2){
            life++;
        }
        else{
            lim_cd -= FRAME_PER_SECOND * 2;
            if(lim_cd < 0){
                lim_cd = 0;
            }
        }
        exp -= EXP_LIM;
    }
}

void Klee::update_ins(int x_){
    mKleeBox.x = x_;
    mKleeVelX = 0; mKleeVelY = 0;
    kleeFrame = 0;
    spriteId = 3;
    if(fLeft) spriteId += 9;
}

void Klee::setState(){
    string fname = "data.txt";
    fstream file(fname);
    vector<string> row;
    vector<vector<string>> content;
    string line, stringId;
    if(file.is_open()){
        while(getline(file, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, stringId, ' ')){
                row.pb(stringId);
            }
            content.pb(row);
        }
        file.close();
    }
    life = stoi(content[0][0]); exp = stoi(content[0][1]); lim_cd = stoi(content[0][2]);
    ins = stoi(content[0][3]); mKleeBox.x = stoi(content[0][4]); mKleeBox.y = stoi(content[0][5]);
    last_ground.fi = mKleeBox.x; last_ground.se = mKleeBox.y;
}

void Klee::updateState(){
    string fname = "data.txt";
    ofstream file(fname);
    if(file.is_open()){
        string line = to_string(life) + " " + to_string(exp) + " " + to_string(lim_cd) + " " + to_string(ins) + " " + to_string(last_ground.fi) + " " + to_string(last_ground.se) + "\n";
        file << line;
        file.close();
    }
}

void Klee::resetcoor(){
    mKleeBox.x = 10; mKleeBox.y = 510;
    last_ground.fi = 10; last_ground.se = 510;
    ins = 1;
}
