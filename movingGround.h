#ifndef MOVINGGROUND_H
#define MOVINGGROUND_H
#include "Utils.h"
#include "Tile.h"
#include "Klee.h"

class movingGround
{
    public:
        movingGround(int Lx, int Rx, int y, int lx, int rx, int ly, int ry, string dire);
        bool onmvGround(Klee* klee);
        void move(Tile* tiles[], Klee* klee);
        int getLT();
        int getRT();
        int getLx();
        int getRx();
        SDL_Rect getBox();
        int getly();
        int getry();

    private:
        int mVelX, mVelY;
        int lim_lx, lim_rx, lim_ly, lim_ry;
        SDL_Rect mBox;
        int dir;
        int LT, RT;
        string d;
};

#endif // MOVINGGROUND_H
