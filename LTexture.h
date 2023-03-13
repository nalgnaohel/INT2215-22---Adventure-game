#ifndef LTEXTURE_H
#define LTEXTURE_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
//Texture transparent
class LTexture
{
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile(std::string path, SDL_Renderer* gRenderer);
        void free(); // deallocate texture
        void render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL); //render at a specific point
        //get size of an image
        int getWidth();
        int getHeight();

    protected:

    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};

#endif // LTEXTURE_H
