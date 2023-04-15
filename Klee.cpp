#include "Klee.h"

Klee::Klee(SDL_Rect& box)
{
    //ctor
    mKleeBox.x = 10; mKleeBox.y = SCREEN_HEIGHT - 64 - box.h;
    mKleeVelX = 0; mKleeVelY = 0;
    kleeFrame = 0;
    mLeft = 0; mRight = 0; mJump = 0; mUp = 0; mDown = 0;
    spriteId = 3;
    statusX = 0; //0: idle, 1: walk left, 2: walk right
    mKleeBox.w = box.w;
    mKleeBox.h = box.h;
    mKleeMxHealth = 100;
    mKleeHealth = mKleeMxHealth;
    wind = 1;
}

void Klee::handleKleeEvent(SDL_Event& e){
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
        case SDLK_UP:
            {
                mJump = 1;
            }
        break;
        case SDLK_RIGHT:
            {
                statusX = 2; mKleeVelX += KLEE_VEL;
                mRight = 1; mLeft = 0; spriteId = 8; kleeFrame = 0;
            }
        break;
        case SDLK_LEFT:
            {
                statusX = 1; mKleeVelX -= KLEE_VEL;
                mLeft = 1; mRight = 0; spriteId = 8; kleeFrame = 0;
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
                mLeft = 0; mKleeVelX = 0; spriteId = 3; kleeFrame = 0;
            }
        break;
        case SDLK_RIGHT:
            {
                mRight = 0; mKleeVelX = 0; spriteId = 3; kleeFrame = 0;
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
    statusX = 0; //0: idle, 1: walk left, 2: walk right
    mKleeBox.w = box.w;
    mKleeBox.h = box.h;
    mKleeMxHealth = 100;
    mKleeHealth = mKleeMxHealth;
    wind = 1;
}

bool Klee::onGround(vector<pair<pair<int, int>, int>>& ground){
    for(int i = 0; i < (int)ground.size(); i++){
        //cout << ground[i].fi.fi * TILE_WIDTH << ' ' << ground[i].fi.se * TILE_WIDTH << ' ' << ground[i].se * TILE_HEIGHT << '\n';
        int lx_ground = ground[i].fi.fi * TILE_WIDTH;
        int rx_ground = (ground[i].fi.se + 1) * TILE_WIDTH;
        int y_ground = ground[i].se * TILE_HEIGHT;
        //cout << mKleeBox
        if(mKleeBox.y + mKleeBox.h == y_ground){
            if(lx_ground <= mKleeBox.x && mKleeBox.x <= rx_ground){
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

void Klee::move(Tile* tiles[], vector<pair<pair<int, int>, int>>& ground){
    if(mJump){
        if(onGround(ground)){
            mnHeight = mKleeBox.y - 8 * TILE_HEIGHT - 16;
        }
        mKleeVelY += KLEE_VEL;
        int py = mKleeBox.y; mKleeBox.y -= mKleeVelY;
        if(mKleeBox.y < max(150, mnHeight)){
            mKleeBox.y = max(150, mnHeight);
            mJump = 0; mKleeVelY = 0;
        }
        for(int i = 0; i < (int)ground.size(); i++){
            int lx_ground = ground[i].fi.fi * TILE_WIDTH;
            int rx_ground = (ground[i].fi.se + 1) * TILE_WIDTH;
            int y_ground = ground[i].se * TILE_HEIGHT;
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
        cout << "Fuck: ";
        mDown = 1;
    }
    if(mDown){
        //cout << mKleeVelY << '\n';
        mKleeVelY += GRAVITY_SPEED;
        if(mKleeVelY > MAX_FALL_SPEED){
            mKleeVelY = MAX_FALL_SPEED;
        }
        int pdy = mKleeBox.y + mKleeBox.h;
        mKleeBox.y += mKleeVelY;
        for(int i = 0; i < (int)ground.size(); i++){
            if(mKleeBox.x + mKleeBox.w >= ground[i].fi.fi * TILE_WIDTH && mKleeBox.x <= (ground[i].fi.se + 1) * TILE_WIDTH){
                if(mKleeBox.y + mKleeBox.h >= ground[i].se * TILE_HEIGHT && pdy <= ground[i].se * TILE_HEIGHT){
                    mKleeVelY = 0; mDown = 0;
                    cout << "Expected to touch here: " << ground[i].fi.fi * TILE_WIDTH << ' ' << ground[i].fi.se * TILE_WIDTH << ' ' << ground[i].se * TILE_HEIGHT << '\n';
                    mKleeBox.y = ground[i].se * TILE_HEIGHT - mKleeBox.h;

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

void Klee::updateKleeBox(SDL_Rect& box){
    mKleeBox.w = box.w;
    mKleeBox.h = box.h;
}

int Klee::getKleeHealth(){
    return mKleeHealth;
}

void Klee::updateKleeHealth(int damage){
    mKleeHealth -= damage;
    if(mKleeHealth <= 0){
        mKleeLife--;
        mKleeHealth = mKleeMxHealth;
    }
}

int Klee::getKleeLife(){
    return mKleeLife;
}

bool Klee::isJump(){
    return mJump;
}

bool Klee::isDown(){
    return mDown;
}
