#ifndef FUNCTION_H
#define FUNCTION_H
#include "Utils.h"
#include "LTexture.h"
#include "Buttons.h"

int showMenu(LTexture& background, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, vector<Buttons>& menuButtons);
int showClearLevel(SDL_Renderer* gRenderer, LTexture& gTEndLevelDialog, LTexture& gTEndLevelText, TTF_Font* gEndLevelFont);
int showGameOver(string gameOverS, LTexture& background, LTexture& gTDialog, LTexture& gTGameOverText, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, TTF_Font* gGameOverFont, vector<Buttons>& gameOverButtons);
#endif // FUNCTION_H
