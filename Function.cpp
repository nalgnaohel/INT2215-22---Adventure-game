#include "Function.h"

int showMenu(Mix_Music* menuMusic, LTexture& background, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, vector<Buttons*>& menuButtons){
    //Handle button event
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int selected[(int)menuButtons.size()];
    for(int i = 0; i < (int)menuButtons.size(); i++){
        selected[i] = 0;
    }
    SDL_Color changedColor = {70, 12, 12};
    SDL_Color originalColor = menuButtons[0]->getColor();
    SDL_Event ev;
    LTexture gTTitle;
    SDL_Color titleColor = {51, 25, 0};
    //if(!gTTitle.loadFromRenderedText("Amazing Adventure", titleColor, gRenderer, )){

    //}
    Mix_PlayMusic(menuMusic, -1);
    while(true){
        background.render(0, 0, gRenderer);
        for(int i = 0; i < (int)menuButtons.size(); i++){
            menuButtons[i]->render(gRenderer, gTButtonsSpriteSheet[selected[i]], NULL);
        }
        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT){
                //Mix_HaltMusic();
                return 1;
            }
            int x, y; SDL_GetMouseState(&x, &y);
            switch(ev.type){
            case SDL_MOUSEMOTION:
            {
                for(int i = 0; i < (int)menuButtons.size(); i++){
                    if(!menuButtons[i]->isInside(x, y)){
                        if(!selected[i]){
                            selected[i] = 1;
                            menuButtons[i]->changeNameColor(changedColor);
                        }
                    }
                    else{
                        if(selected[i]){
                            selected[i] = 0;
                            menuButtons[i]->changeNameColor(originalColor);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                for(int i = 0; i < (int)menuButtons.size(); i++){
                    if(menuButtons[i]->isInside(x, y)){
                        cout << i << '\n';
                        Mix_HaltMusic();
                        return i;
                    }
                }
            }
            break;
            default:
                break;
            }
        }
        SDL_RenderPresent(gRenderer);
    }
    return 1;
}

int showClearLevel(Mix_Music* wonMusic, string won_s, LTexture& background, LTexture& gTDialog, LTexture& gTWonText, TTF_Font* gWonFont,  LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, vector<Buttons*>& wonButtons){
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int selected[(int)wonButtons.size()];
    for(int i = 0; i < (int)wonButtons.size(); i++){
        selected[i] = 0;
    }
    SDL_Color changedColor = {70, 12, 12};
    SDL_Color originalColor = wonButtons[0]->getColor();
    SDL_Event ev;
    Mix_PlayMusic(wonMusic, -1);
    while(true){
        background.render(0, 0, gRenderer);
        gTDialog.render(0, SCREEN_HEIGHT / 2 - 272, gRenderer);
        if(!gTWonText.loadFromRenderedText(won_s, changedColor, gRenderer, gWonFont)){
            cout << "Unable to display Game Over! Error: " << TTF_GetError() << '\n';
            return 1;
        }
        else{
            int w = gTWonText.getWidth(); int h = gTWonText.getHeight();
            gTWonText.render(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2 - 50, gRenderer);
        }
        for(int i = 0; i < (int)wonButtons.size(); i++){
            wonButtons[i]->render(gRenderer, gTButtonsSpriteSheet[selected[i]], NULL);
        }
        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT){
                Mix_HaltMusic();
                return 1;
            }
            int x, y; SDL_GetMouseState(&x, &y);
            switch(ev.type){
            case SDL_MOUSEMOTION:
            {
                for(int i = 0; i < (int)wonButtons.size(); i++){
                    if(!wonButtons[i]->isInside(x, y)){
                        if(!selected[i]){
                            selected[i] = 1;
                            wonButtons[i]->changeNameColor(changedColor);
                        }
                    }
                    else{
                        if(selected[i]){
                            selected[i] = 0;
                            wonButtons[i]->changeNameColor(originalColor);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                for(int i = 0; i < (int)wonButtons.size(); i++){
                    if(wonButtons[i]->isInside(x, y)){
                        Mix_HaltMusic();
                        return i;
                    }
                }
            }
            break;
            default:
                break;
            }
        }
        SDL_RenderPresent(gRenderer);
    }
    return 1;
}

int showGameOver(Mix_Music* gameOverMusic, string gameOverS, LTexture& background, LTexture& gTDialog, LTexture& gTGameOverText, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, TTF_Font* gGameOverFont, vector<Buttons*>& gameOverButtons){
    //Handle button event
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int selected[(int)gameOverButtons.size()];
    for(int i = 0; i < (int)gameOverButtons.size(); i++){
        selected[i] = 0;
    }
    SDL_Color changedColor = {70, 12, 12};
    SDL_Color originalColor = gameOverButtons[0]->getColor();
    SDL_Event ev;
    LTexture gTTitle;
    Mix_PlayMusic(gameOverMusic, -1);
    while(true){
        background.render(0, 0, gRenderer);
        gTDialog.render(0, SCREEN_HEIGHT / 2 - 272, gRenderer);
        if(!gTGameOverText.loadFromRenderedText(gameOverS, changedColor, gRenderer, gGameOverFont)){
            cout << "Unable to display Game Over! Error: " << TTF_GetError() << '\n';
            return 1;
        }
        else{
            int w = gTGameOverText.getWidth(); int h = gTGameOverText.getHeight();
            gTGameOverText.render(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2 - 50, gRenderer);
        }
        for(int i = 0; i < (int)gameOverButtons.size(); i++){
            gameOverButtons[i]->render(gRenderer, gTButtonsSpriteSheet[selected[i]], NULL);
        }
        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT){
                Mix_HaltMusic();
                return 1;
            }
            int x, y; SDL_GetMouseState(&x, &y);
            switch(ev.type){
            case SDL_MOUSEMOTION:
            {
                for(int i = 0; i < (int)gameOverButtons.size(); i++){
                    if(!gameOverButtons[i]->isInside(x, y)){
                        if(!selected[i]){
                            selected[i] = 1;
                            gameOverButtons[i]->changeNameColor(changedColor);
                        }
                    }
                    else{
                        if(selected[i]){
                            selected[i] = 0;
                            gameOverButtons[i]->changeNameColor(originalColor);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                for(int i = 0; i < (int)gameOverButtons.size(); i++){
                    if(gameOverButtons[i]->isInside(x, y)){
                        Mix_HaltMusic();
                        return i;
                    }
                }
            }
            break;
            default:
                break;
            }
        }
        SDL_RenderPresent(gRenderer);
    }
    return 1;
}


int showInstruction(Mix_Chunk* page, LTexture& background, LTexture& gTEndLevelDialog, LTexture (&gTInstruct)[INSTR_LENGTH], SDL_Renderer* gRenderer){
    while(true){
        background.render(0, 0, gRenderer);
        gTEndLevelDialog.render(0, SCREEN_HEIGHT / 2 - 272, gRenderer);
        int cur_y = SCREEN_HEIGHT /2 - 250;
        for(int i = 0; i < INSTR_LENGTH; i++){
            if(gTInstruct[i].getWidth() > 0){
                gTInstruct[i].render(50, cur_y, gRenderer);
                cur_y += 5 + gTInstruct[i].getHeight();
            }
        }
        SDL_Event ev;
        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT){
                return 1;
            }
            if(ev.type == SDL_KEYDOWN && ev.key.repeat == 0){
                switch(ev.key.keysym.sym){
                case SDLK_KP_ENTER:
                {
                    Mix_PlayChannel(-1, page, 0);
                    return 0;
                }
                break;
                case SDLK_SPACE:
                {
                    Mix_PlayChannel(-1, page, 0);
                    return 0;
                }
                break;
                default:
                break;
                }
            }
        }
        SDL_RenderPresent(gRenderer);
    }
    return 1;
}

int showFail(Mix_Music* gameOverMusic, string fail_s, LTexture& background, LTexture& gTDialog, LTexture& gTFailedText, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, TTF_Font* gFailedFont, vector<Buttons*>& failedButtons){
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int selected[(int)failedButtons.size()];
    for(int i = 0; i < (int)failedButtons.size(); i++){
        selected[i] = 0;
    }
    SDL_Color changedColor = {70, 12, 12};
    SDL_Color originalColor = failedButtons[0]->getColor();
    SDL_Event ev;
    Mix_PlayMusic(gameOverMusic, -1);
    while(true){
        background.render(0, 0, gRenderer);
        gTDialog.render(0, SCREEN_HEIGHT / 2 - 272, gRenderer);
        if(!gTFailedText.loadFromRenderedText(fail_s, changedColor, gRenderer, gFailedFont)){
            cout << "Unable to display Game Over! Error: " << TTF_GetError() << '\n';
            return 1;
        }
        else{
            int w = gTFailedText.getWidth(); int h = gTFailedText.getHeight();
            gTFailedText.render(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2 - 50, gRenderer);
        }
        for(int i = 0; i < (int)failedButtons.size(); i++){
            failedButtons[i]->render(gRenderer, gTButtonsSpriteSheet[selected[i]], NULL);
        }
        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT){
                Mix_HaltMusic();
                return 1;
            }
            int x, y; SDL_GetMouseState(&x, &y);
            switch(ev.type){
            case SDL_MOUSEMOTION:
            {
                for(int i = 0; i < (int)failedButtons.size(); i++){
                    if(!failedButtons[i]->isInside(x, y)){
                        if(!selected[i]){
                            selected[i] = 1;
                            failedButtons[i]->changeNameColor(changedColor);
                        }
                    }
                    else{
                        if(selected[i]){
                            selected[i] = 0;
                            failedButtons[i]->changeNameColor(originalColor);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                for(int i = 0; i < (int)failedButtons.size(); i++){
                    if(failedButtons[i]->isInside(x, y)){
                        Mix_HaltMusic();
                        return i;
                    }
                }
            }
            break;
            default:
                break;
            }
        }
        SDL_RenderPresent(gRenderer);
    }
    return 1;
}
