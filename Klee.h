#ifndef KLEE_H
#define KLEE_H
#include "Utils.h"
#include "LTexture.h"
#include "Tile.h"

class Klee
{
    public:
        static constexpr double KLEE_WIDTH = 32.5;
        static constexpr double KLEE_HEIGHT = 48;
        static const int KLEE_VEL = 3;
        static const int KLEE_VEL_SPEED = 9;

        Klee(SDL_Rect& box);

        void handleKleeEvent(SDL_Event &e, SDL_Rect (&gKleeSpriteClips)[9][12], vector<pair<pair<int, int>, int>>& ground);
        bool touchObjects(Tile* tiles[]);
        bool onGround(vector<pair<pair<int, int>, int>>& ground);
        void move(Tile* tiles[],vector<pair<pair<int, int>, int>>& ground, SDL_Rect (&gKleeSpriteClips)[9][12]);
        void render(LTexture& gTKleeSpriteSheet, SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera);
        void setCamera(SDL_Rect& camera);
        void updateKleeHealth(int damage);
        int getKleeVelX();
        int getKleeVelY();
        SDL_Rect getKleeBox();
        int getKleeLife();
        int getKleeHealth();
        int spriteId;
        int kleeFrame;
        void updateKleeBox(SDL_Rect& box, vector<pair<pair<int, int>, int>>& ground);
        bool isJump();
        bool isDown();
        void reset(SDL_Rect& box);
        int TOTAL_TILES;
        int wind;

    private:
        int mKleeVelX, mKleeVelY;
        //float mKleeX, mKleeY;
        int mnHeight;
        SDL_Rect mKleeBox;
        int mKleeHealth;
        int mKleeLife;
        int mKleeMxHealth;
        bool mLeft, mRight, mJump, mUp, mDown;
        int statusX;
};

#endif // KLEE_H
