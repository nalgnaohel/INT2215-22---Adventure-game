#ifndef KLEE_H
#define KLEE_H
#include <SDL.h>
#include <SDL_image.h>
#include "LTexture.h"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int LEVEL_WIDTH = 3584;
const int LEVEL_HEIGHT = 640;
const int KLEE_WALKING_FRAME = 4;

class Klee
{
    public:
        static constexpr double KLEE_WIDTH = 32.5;
        static constexpr double KLEE_HEIGHT = 48;
        static const int KLEE_VEL = 3;

        Klee();

        void handleKleeEvent(SDL_Event &e);
        bool checkCollision(SDL_Rect a, SDL_Rect b);
        void move(SDL_Rect& object);
        void render(LTexture& gTKleeSpriteSheet, SDL_Renderer* gRenderer, SDL_Rect* clip, int camX, int camY, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        int getKleePosX();
        int getKleePosY();

    protected:

    private:
        int mKleePosX, mKleePosY;
        int mKleeVelX, mKleeVelY;
        SDL_Rect mKleeCollider;
};

#endif // KLEE_H
