#include "Function.h"

int showMenu(LTexture& background, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, vector<Buttons>& menuButtons){
    //Handle button event
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int selected[(int)menuButtons.size()];
    for(int i = 0; i < (int)menuButtons.size(); i++){
        selected[i] = 0;
    }
    SDL_Color changedColor = {70, 12, 12};
    SDL_Color originalColor = menuButtons[0].getColor();
    SDL_Event ev;
    LTexture gTTitle;
    SDL_Color titleColor = {51, 25, 0};
    //if(!gTTitle.loadFromRenderedText("Amazing Adventure", titleColor, gRenderer, )){

    //}
    while(true){

        background.render(0, 0, gRenderer);
        for(int i = 0; i < (int)menuButtons.size(); i++){
            menuButtons[i].render(gRenderer, gTButtonsSpriteSheet[selected[i]], NULL);
        }
        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT){
                return 1;
            }
            int x, y; SDL_GetMouseState(&x, &y);
            switch(ev.type){
            case SDL_MOUSEMOTION:
            {
                for(int i = 0; i < (int)menuButtons.size(); i++){
                    if(!menuButtons[i].isInside(x, y)){
                        if(!selected[i]){
                            selected[i] = 1;
                            menuButtons[i].changeNameColor(changedColor);
                        }
                    }
                    else{
                        if(selected[i]){
                            selected[i] = 0;
                            menuButtons[i].changeNameColor(originalColor);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                for(int i = 0; i < (int)menuButtons.size(); i++){
                    if(menuButtons[i].isInside(x, y)){
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

int showClearLevel(SDL_Renderer* gRenderer, LTexture& gTEndLevelDialog, LTexture& gTEndLevelText, TTF_Font* gEndLevelFont){
    SDL_RenderClear(gRenderer);
    gTEndLevelDialog.render(SCREEN_WIDTH / 2 - gTEndLevelDialog.getWidth() / 2, SCREEN_HEIGHT / 2 - gTEndLevelDialog.getHeight() / 2, gRenderer);
    SDL_Color cl = {0, 0, 0};
    if(!gTEndLevelText.loadFromRenderedText("Congratulations for clearing this level\n", cl, gRenderer, gEndLevelFont)){
        cout << "Unable to display end level text! Error: " << TTF_GetError() << "\n";
        return 0;
    }
    else{
        gTEndLevelText.render(SCREEN_WIDTH / 2 - gTEndLevelText.getWidth() / 2, SCREEN_HEIGHT / 2 - gTEndLevelText.getHeight(), gRenderer);
        return 1;
    }

}

int showGameOver(string gameOverS, LTexture& background, LTexture& gTDialog, LTexture& gTGameOverText, LTexture (&gTButtonsSpriteSheet)[2], SDL_Renderer* gRenderer, TTF_Font* gGameOverFont, vector<Buttons>& gameOverButtons){
    //Handle button event
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int selected[(int)gameOverButtons.size()];
    for(int i = 0; i < (int)gameOverButtons.size(); i++){
        selected[i] = 0;
    }
    SDL_Color changedColor = {70, 12, 12};
    SDL_Color originalColor = gameOverButtons[0].getColor();
    SDL_Event ev;
    LTexture gTTitle;
    SDL_Color titleColor = {51, 25, 0};
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
            gameOverButtons[i].render(gRenderer, gTButtonsSpriteSheet[selected[i]], NULL);
        }
        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT){
                return 1;
            }
            int x, y; SDL_GetMouseState(&x, &y);
            switch(ev.type){
            case SDL_MOUSEMOTION:
            {
                for(int i = 0; i < (int)gameOverButtons.size(); i++){
                    if(!gameOverButtons[i].isInside(x, y)){
                        if(!selected[i]){
                            selected[i] = 1;
                            gameOverButtons[i].changeNameColor(changedColor);
                        }
                    }
                    else{
                        if(selected[i]){
                            selected[i] = 0;
                            gameOverButtons[i].changeNameColor(originalColor);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                for(int i = 0; i < (int)gameOverButtons.size(); i++){
                    if(gameOverButtons[i].isInside(x, y)){
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
