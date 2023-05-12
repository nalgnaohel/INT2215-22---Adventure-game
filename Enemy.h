#ifndef ENEMY_H
#define ENEMY_H
#include "Utils.h"
#include "LTexture.h"
#include "Tile.h"
#include "Arrow.h"
#include "Healthbar.h"

class Enemy
{
    public:
        Enemy(int lx, int y, int rx, int type_, SDL_Rect& box);
        ~Enemy();
        bool isNearKlee(SDL_Rect& kBox);
        void move(int vel);
        void action(Mix_Music* playMusic_fight, Mix_Chunk* stab, Mix_Chunk* ghost_att, SDL_Rect& kBox, SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]);
        void render(LTexture& gTEnemyHb, LTexture& gTEnemyHbBg, SDL_Rect (&gEnemyHbClips)[2], LTexture& gTEnemy, SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera);
        void updateEnemyBox(SDL_Rect& box);
        int getEnemyVel();
        SDL_Rect getEnemyBox();
        int getEnemyDamage();
        int getEnemyLeftX();
        int getEnemyRightX();
        int getEnemyGround();
        int getExp_val();
        void updateHealth(Mix_Chunk* ghost_die[2], int d, SDL_Rect (&gEnemySpriteClips)[2][12][7]);
        int getHealth();
        int getDeltaX();
        int type;
        int spriteId;
        int frame;
        int dir;
        vector<Arrow*> mArrowList;
        Healthbar* ghost_hbar;
        void updateFrame(Mix_Music* playMusic_fight, Mix_Chunk* stab, Mix_Chunk* ghost_att, SDL_Rect &kbox, SDL_Rect (&gEnemySpriteClips)[2][12][7], int (&gEnemySpriteClipsSize)[2][12]);
        void setHealth(int& cur_health); //this void is only used for load
        void setDeltaX(int& delta);

    private:
        int mEnemyLeftX, mEnemyRightX;
        int mEnemyVel;
        int mEnemyRunVel;
        SDL_Rect mEnemyBox;
        int damage;
        bool att, scream, run, walk;
        bool fLeft, fRight;
        int gr;
        int exp_val;
        int prev_state;
        int health, mxHealth;
        int del;
};

#endif // KLEE_H
