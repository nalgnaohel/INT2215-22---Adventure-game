#include "Tile.h"

Tile::Tile(int x, int y, int tileType)
{
    //ctor
    mBox.x = x; mBox.y = y;
    mBox.w = TILE_WIDTH; mBox.h = TILE_HEIGHT;
    mType = tileType;
}

void Tile::render(SDL_Renderer* gRenderer, LTexture (&gTypeTiles)[TOTAL_TILE_TYPES], SDL_Rect& camera){
    if(checkCollision(mBox, camera)){
        //cout << mType << '\n';
        gTypeTiles[mType].render(mBox.x - camera.x, mBox.y - camera.y, gRenderer);
    }
}

int Tile::getType(){
    return mType;
}

SDL_Rect Tile::getBox(){
    return mBox;
}
