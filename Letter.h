#ifndef LETTER_H
#define LETTER_H
#include "Utils.h"
#include "LTexture.h"

class Letter
{
    public:
        Letter(int x_, SDL_Rect& box);
        virtual ~Letter();
        void move();
        bool open;
        void render(SDL_Renderer* gRenderer, LTexture& gTLetter, SDL_Rect* gLetter);
        SDL_Rect getBox();

    protected:

    private:
        SDL_Rect mBox; int dir;
        int mLetterVel;
};

#endif // LETTER_H
