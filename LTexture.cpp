#include "LTexture.h"

LTexture::LTexture()
{
    //ctor
    //Initialize
    mTexture = NULL;
    mHeight = 0; mWidth = 0;
}

LTexture::~LTexture()
{
    //dtor
    //Deallocate
    free();
}

bool LTexture::loadFromFile(std:: string path, SDL_Renderer* gRenderer){
    //result texture
    SDL_Texture* resTexture = NULL;
    //Load image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        cout << "Unable to load image " << path.c_str() << "! SDL_Image Error: " << IMG_GetError() << '\n';
    }
    else{
        //color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
        //create from surface pixels
        resTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(resTexture == NULL){
            cout << "Unable to create texture from surface " << path.c_str() << "! SDL Error: " << SDL_GetError() << '\n';
        }
        else{
            mWidth = loadedSurface->w; mHeight = loadedSurface->h;
        }
        //Destroy old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = resTexture;
    return mTexture != NULL;
}

void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mWidth = 0; mHeight = 0;
        mTexture = NULL;
    }
}

void LTexture::render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
    //Set rendering space and render
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}
