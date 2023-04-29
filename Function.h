#ifndef FUNCTION_H
#define FUNCTION_H
#include "Utils.h"
#include "LTexture.h"
#include "Buttons.h"

int showMenu(Mix_Music* menuMusic, LTexture& background, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, vector<Buttons*>& menuButtons);
int showClearLevel(Mix_Music* wonMusic, string won_s, LTexture& background, LTexture& gTDialog, LTexture& gTWonText, TTF_Font* gWonFont,  LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, vector<Buttons*>& wonButtons);
int showGameOver(Mix_Music* gameOverMusic, string gameOverS, LTexture& background, LTexture& gTDialog, LTexture& gTGameOverText, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, TTF_Font* gGameOverFont, vector<Buttons*>& gameOverButtons);
int showInstruction(Mix_Chunk* page, LTexture& background, LTexture& gTEndLevelDialog, LTexture (&gTInstruct)[INSTR_LENGTH], SDL_Renderer* gRenderer);
int showFail(Mix_Music* gameOverMusic, string fail_s, LTexture& background, LTexture& gTDialog, LTexture& gTFailedText, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, TTF_Font* gFailedFont, vector<Buttons*>& failedButtons);
#endif // FUNCTION_H
