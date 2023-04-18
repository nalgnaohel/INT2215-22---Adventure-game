#ifndef TILE_H
#define TILE_H
#include "Utils.h"
#include "LTexture.h"

class Tile
{
    public:
        Tile();
        Tile(int x, int y, int tileType);
        void render(SDL_Renderer* gRenderer, LTexture (&gTileTypes)[TOTAL_TILE_TYPES], SDL_Rect &camera);
        int getType();
        SDL_Rect getBox();
        void updateBox(int x, int y);

    private:
        SDL_Rect mBox;
        int mType;
};

#endif // TILE_H
