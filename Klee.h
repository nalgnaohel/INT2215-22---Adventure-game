#ifndef KLEE_H
#define KLEE_H
#include "Utils.h"
#include "LTexture.h"
#include "Tile.h"
#include "Arrow.h"
#include "Enemy.h"
#include "Healthbar.h"

class Klee
{
    public:
        static const int KLEE_VEL = 3;
        static const int KLEE_VEL_SPEED = 9;

        Klee(SDL_Rect& box);
        ~Klee();

        void handleKleeEvent(SDL_Event &e, SDL_Rect (&gKleeSpriteClips)[18][16], vector<pair<pair<int, int>, int>>& ground, pair<int, int> (&kFeet)[9][16]);
        bool touchObjects(Tile* tiles[]);
        bool onGround(vector<pair<pair<int, int>, int>>& ground);
        void move(int curLevel, Tile* tiles[],vector<pair<pair<int, int>, int>>& ground, SDL_Rect (&gKleeSpriteClips)[18][16], pair<int, int> (&kFeet)[9][16]);
        void render(LTexture& gTKleeSpriteSheet, LTexture& gTKleeHb, LTexture& gTKleeHbBackground, SDL_Rect (&gKleeHbClips)[2], LTexture& gTKleeExp, LTexture& gTKleeExpBg, SDL_Rect (&gKleeExpbClips)[2], SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera);
        void setCamera(int curLevel, SDL_Rect& camera);
        void updateKleeHealth(Mix_Chunk* pState, int d, SDL_Rect (&gKleeSpriteClips)[18][16]);
        void updateExp(Mix_Chunk* lv_up, int val);
        int getKleeVelX();
        int getKleeVelY();
        bool isJump();
        SDL_Rect getKleeBox();
        int getKleeHealth();
        int spriteId;
        int kleeFrame;
        void updateKleeBox(SDL_Rect& box, vector<pair<pair<int, int>, int>>& ground, pair<int, int> (&kFeet)[9][16]);
        void handleArrowList(Mix_Chunk* ghost_die[2], SDL_Renderer* gRenderer, LTexture (&gTArrow)[6], SDL_Rect (&gArrowSpriteClips)[6][9], int (&gArrowSpriteClipsSize)[6], SDL_Rect& camera, vector<Enemy*> (&ghost)[TOTAL_ENEMIES], SDL_Rect (&gEnemySpriteClips)[2][12][7]);
        bool isDown();
        int TOTAL_TILES;
        int st3;
        int s_cd;
        int life;
        pair<int, int> feet;
        void updatemvGround(int& y);
        vector<Arrow*> mArrowList;
        void updateFrame(SDL_Rect (&gKleeSpriteClips)[18][16], int (&gKleeSpriteClipsSize)[18], vector<pair<pair<int, int>, int>>& ground, SDL_Rect (&gArrowSpriteClips)[6][9], SDL_Rect& camera, pair<int, int> (&kFeet)[9][16]);

    private:
        int mKleeVelX, mKleeVelY;
        int mnHeight;
        SDL_Rect mKleeBox;
        int mKleeHealth;
        int mKleeMxHealth;
        bool mLeft, mRight, mJump, mUp, mDown;
        int mmvLx, mmvRx, mmvy;
        int fLeft, fRight;
        Healthbar* klee_hbar;
        int exp;
        int lim_cd;
        int level;
        Healthbar* klee_exp_bar; //well actually this is an exp bar
};

#endif // KLEE_H
