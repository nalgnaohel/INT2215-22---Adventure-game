#include "Buttons.h"

Buttons::Buttons(int w, int h)
{
    //ctor
    mButtonBox.x = 0; mButtonBox.y = 0;
    mButtonBox.w = w; mButtonBox.h = h;
    buttonName = "";
}

Buttons::~Buttons(){
    mTButtonName.free();
}

void Buttons::setPosition(int x, int y){
    mButtonBox.x = x; mButtonBox.y = y;
}

void Buttons::setButtonName(string name, SDL_Color textColor, TTF_Font* font){
    buttonName = name; buttonNameColor = textColor; buttonNameFont = font;
}

bool Buttons::isInside(int& x, int& y){
    bool inside = true;
    if(x < mButtonBox.x){
        inside = false;
    }
    if(x > mButtonBox.x + mButtonBox.w){
        inside = false;
    }
    if(y < mButtonBox.y){
        inside = false;
    }
    if(y > mButtonBox.y + mButtonBox.h){
        inside = false;
    }
    return inside;
}

void Buttons::render(SDL_Renderer* gRenderer, LTexture& gTButtonsSpriteSheet, SDL_Rect* clip){
    gTButtonsSpriteSheet.render(mButtonBox.x, mButtonBox.y, gRenderer, clip);
    if(buttonName != ""){
        if(!mTButtonName.loadFromRenderedText(buttonName, buttonNameColor, gRenderer, buttonNameFont)){
            cout << "Unable to display button name!\n";
        }
        else{
            mTButtonName.render(mButtonBox.x + (mButtonBox.w / 2 - mTButtonName.getWidth() / 2), mButtonBox.y + (mButtonBox.h / 2 - mTButtonName.getHeight() / 2), gRenderer);
        }
    }
}

void Buttons::changeNameColor(SDL_Color newNameColor){
    buttonNameColor = newNameColor;
}

SDL_Color Buttons::getColor(){
    return buttonNameColor;
}

SDL_Rect Buttons::getButtonBox(){
    return mButtonBox;
}

string Buttons::getButtonName(){
    return buttonName;
}
