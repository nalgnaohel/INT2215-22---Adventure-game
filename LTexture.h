#ifndef LTEXTURE_H
#define LTEXTURE_H
#include "Utils.h"
using namespace std;
//Texture transparent
class LTexture
{
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile(string path, SDL_Renderer* gRenderer);
        #if defined(SDL_TTF_MAJOR_VERSION)
        bool loadFromRenderedText(string textureText, SDL_Color textColor, SDL_Renderer* gRenderer, TTF_Font* gFont);
        #endif
        void free(); // deallocate texture
        void render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE); //render at a specific point
        //get size of an image
        int getWidth();
        int getHeight();
        //string f_name;

    protected:

    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

#endif // LTEXTURE_H
