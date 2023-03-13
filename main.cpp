#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include "LTexture.h"
#include "Button.h"
using namespace std;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const string WINDOW_TITLE = "Game ver 1.0";

SDL_Texture* loadTexture(string path);
bool initSDL();
bool loadMedia();
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void close();

SDL_Window* gWindow = NULL;
LTexture gTBackground;
LTexture gTKlee;
SDL_Renderer* gRenderer = NULL;
SDL_Event event;

//Buttons
Button gButtons[TOTAL_BUTTONS];

int main(int argc, char* argv[]){
    if(!initSDL()){
        cout << "Unable to initialize\n";
    }
    else{
        if(!loadMedia()){
            cout << "Unable to load media\n";
        }
        else{
            bool isRunning = 1; double degree = 0;
            while(isRunning){
                while(SDL_PollEvent(&event)){
                    if(event.type == SDL_QUIT){
                        isRunning = 0;
                    }

                    //Button handle events
                    for(int i = 0; i < TOTAL_BUTTONS; i++){
                        gButtons[i].handleEvent(&event);
                    }
                }
                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render texture to screen
				gTBackground.render(0, 0, gRenderer);
				gTKlee.render(10, 458, gRenderer);
				for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++){
                    gButtons[i].render(gTButtonSprite, gRenderer, NULL);
				}
				//Update screen
				SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}

bool initSDL(){
    bool success = true;
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
        cout << "Unable to initialize\n"; success = false;
    }
    else{
        if(!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" )){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

        //Create window
        gWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(gWindow == NULL){
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else{
            //Get window surface
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL){
                cout << "Could not create renderer\n"; success = false;
            }
            else{
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //init IMG loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) && imgFlags)){
                    cout << "Could not initialize SDL_Image\n"; success = false;
                }
            }
        }

    }
    return success;
}

bool loadMedia(){
    bool success = true;
    if(!gTBackground.loadFromFile("image/bg.png", gRenderer)){
        cout << "Failed to load background image!"; success = false;
    }
    if(!gTKlee.loadFromFile("image/klee_test.png", gRenderer)){
        cout << "Failed to load Klee image!"; success = false;
    }
    if(!gTButtonSprite.loadFromFile("image/button.png", gRenderer)){
        cout << "Failed to load buttons!"; success = false;
    }
    else{
        //Top left
        for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++){
            gSpriteClips[i].x = 0;
            gSpriteClips[i].y = i * 200;
            gSpriteClips[i].w = BUTTON_WIDTH;
            gSpriteClips[i].h = BUTTON_HEIGHT;
        }
        gButtons[0].setPosition(0, 0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
    }
    return success;
}

void close(){
	//Free loaded image
	gTButtonSprite.free();
	gTBackground.free();
	gTKlee.free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL; gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
