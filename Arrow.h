#ifndef ARROW_H
#define ARROW_H
#include "Utils.h"
#include "LTexture.h"

class Arrow
{
    public:
        Arrow(int a_type);
        ~Arrow();
        void move(int x_lim);
        int frame;
        int getmVelX();
        bool isActive();
        void setArrowBox(int x, int y, SDL_Rect (&gArrowSpriteClips)[6][9]);
        void render(SDL_Renderer* gRenderer, LTexture& gTArrow, SDL_Rect* clip, SDL_Rect& camera);
        SDL_Rect getBox();
        int type;//0: arrow, 1: sphere, 2: enemy_charge -1: none
        void setDir(int d);
        bool hit;
        int getDamage();
        void updateFrame(SDL_Rect (&gArrowSpriteClips)[6][9], int (&gArrowSpriteClipsSize)[6]);

    private:
        SDL_Rect mBox;
        int mVelX;
        bool active;
        int damage;
        int dir; //-1 ->left, 1 -> right
};

#endif // ARROW_H
