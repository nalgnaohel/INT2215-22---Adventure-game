#ifndef ENEMY_H
#define ENEMY_H
#include "Utils.h"
#include "LTexture.h"
#include "Tile.h"

class Enemy
{
    public:
        static constexpr double ENEMY_WIDTH = 41;
        static constexpr double ENEMY_HEIGHT = 49;
        static const int ENEMY_VEL = 1;

        Enemy(int lx, int y, int rx);

        //void handleEnemyEvent(SDL_Event &e);
        void move();
        void render(LTexture& gTEnemy, SDL_Renderer* gRenderer, SDL_Rect& camera);
        int getEnemyVel();
        SDL_Rect getEnemyBox();
        int getEnemyDamage();
        int getEnemyLeftX();
        int getEnemyRightX();
        bool dir;

    protected:

    private:
        int mEnemyVel;
        int mEnemyLeftX, mEnemyRightX;
        SDL_Rect mEnemyBox;
        int damage;
};

#endif // KLEE_H
