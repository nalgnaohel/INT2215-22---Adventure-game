#ifndef KLEE_H
#define KLEE_H
#include <SDL.h>
#include <SDL_image.h>
#include "Utils.h"
#include "LTexture.h"
#include "Tile.h"

class Klee
{
    public:
        static constexpr double KLEE_WIDTH = 32.5;
        static constexpr double KLEE_HEIGHT = 48;
        static const int KLEE_VEL = 3;

        Klee();

        void handleKleeEvent(SDL_Event &e);
        bool touchObjects(Tile* tiles[]);
        void move(Tile* tiles[]);
        void render(LTexture& gTKleeSpriteSheet, SDL_Renderer* gRenderer, SDL_Rect* clip, SDL_Rect& camera);
        void setCamera(SDL_Rect& camera);
        int getKleeVelX();
        int getKleeVelY();
        SDL_Rect getKleeBox();

    protected:

    private:
        int mKleeVelX, mKleeVelY;
        SDL_Rect mKleeBox;
};

#endif // KLEE_H
