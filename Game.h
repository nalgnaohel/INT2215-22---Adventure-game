#ifndef GAME_H
#define GAME_H
#include "Utils.h"
#include "LTexture.h"
#include "Klee.h"
#include "Enemy.h"
#include "Buttons.h"
#include "Function.h"
#include "Timer.h"

class Game
{
    public:
        Game(string level, SDL_Rect& box);
        ~Game();
        bool setTiles();
        int run(SDL_Renderer* gRenderer);
        bool isRunning;

    private:
        string lv;
        vector<Enemy> mush1;
        vector<pair<pair<int, int>, int>> ground;
        Tile* tiles[MAX_TOTAL_TILES];
        int TOTAL_TILES;
        Klee* klee;
        SDL_Rect camera;
};

#endif // GAME_H
