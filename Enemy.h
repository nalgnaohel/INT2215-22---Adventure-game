#ifndef ENEMY_H
#define ENEMY_H
#include "Utils.h"
#include "LTexture.h"
#include "Tile.h"
#include "Arrow.h"

class Enemy
{
    public:
        Enemy(int lx, int y, int rx, int type_, SDL_Rect& box);
        ~Enemy();
        bool isNearKlee(SDL_Rect& kBox);
        void move(int vel);
        void action(SDL_Rect& kBox, SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]);
        void render(LTexture& gTEnemy, SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera);
        void updateEnemyBox(SDL_Rect& box);
        int getEnemyVel();
        SDL_Rect getEnemyBox();
        int getEnemyDamage();
        int getEnemyLeftX();
        int getEnemyRightX();
        void updateHealth(int d, SDL_Rect (&gEnemySpriteClips)[2][12][7]);
        int getHealth();
        int type;
        int spriteId;
        int frame;
        int dir;
        vector<Arrow*> mArrowList;
        void updateFrame(SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]);

    private:
        int mEnemyLeftX, mEnemyRightX;
        int mEnemyVel;
        int mEnemyRunVel;
        SDL_Rect mEnemyBox;
        int damage;
        bool att, scream, run, walk;
        bool fLeft, fRight;
        int gr;
        int health, mxHealth;
};

#endif // KLEE_H
