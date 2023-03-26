#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include "LTexture.h"
#include "Klee.h"
//#include "Button.h"
using namespace std;

const string WINDOW_TITLE = "Game ver 1.0";

SDL_Texture* loadTexture(string path);
bool initSDL();
bool loadMedia();
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void close();


SDL_Window* gWindow = NULL;
LTexture gTBackground;
LTexture gTKlee;
LTexture gTKleeSpriteSheet;
SDL_Rect gKleeSpriteClips[KLEE_WALKING_FRAME];
SDL_Renderer* gRenderer = NULL;
SDL_Event event;
LTexture gTGround;
LTexture gTMap;

//Buttons
//Button gButtons[TOTAL_BUTTONS];

int main(int argc, char* argv[]){
    if(!initSDL()){
        cout << "Unable to initialize\n";
    }
    else{
        if(!loadMedia()){
            cout << "Unable to load media\n";
        }
        else{
            bool isRunning = 1; //double degree = 0;
            Klee klee;
            int frame = 0;
            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            int scrollingOffset = 0;
            SDL_Rect wall; wall.x = 0; wall.y = SCREEN_HEIGHT - 64; wall.w = 32 * 16; wall.h = 64;
            while(isRunning){
                while(SDL_PollEvent(&event)){
                    if(event.type == SDL_QUIT){
                        isRunning = 0;
                    }
                    else if(event.type == SDL_KEYDOWN){
                        switch(event.key.keysym.sym){
                            case SDLK_RIGHT:
                                frame = 0;
                                break;
                            case SDLK_LEFT:
                                frame = 1;
                                break;
                        }
                    }
                    klee.handleKleeEvent(event);
                }
                klee.move(wall);

                --scrollingOffset;
                if(scrollingOffset < -gTBackground.getWidth()){
                    scrollingOffset = 0;
                }
                camera.x = klee.getKleePosX() - Klee::KLEE_WIDTH;
                camera.y = klee.getKleePosY() - Klee::KLEE_HEIGHT;

                //Keep the camera in bounds
                if(camera.x < 0){
                    camera.x = 0;
                }
                if(camera.y < 0){
                    camera.y = 0;
                }
                if(camera.x > LEVEL_WIDTH - camera.w){
                    camera.x = LEVEL_WIDTH - camera.w;
                }
                if(camera.y > LEVEL_HEIGHT - camera.h){
                    camera.y = LEVEL_HEIGHT - camera.h;
                }
                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

                gTBackground.render(scrollingOffset, 0, gRenderer);
                gTBackground.render(scrollingOffset + gTBackground.getWidth(), 0, gRenderer);
				gTMap.render(0, 0, gRenderer, &camera);;
				/*for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++){
                    gButtons[i].render(gTButtonSprite, gRenderer, NULL);
				}*/
				//Update screen
				SDL_Rect* currentClip = &gKleeSpriteClips[frame];
				klee.render(gTKleeSpriteSheet, gRenderer, currentClip, camera.x, camera.y);
				cout << klee.getKleePosX() << ' ' << klee.getKleePosY() << '\n';
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
    if(!gTBackground.loadFromFile("image/png/BG/BG.png", gRenderer)){
        cout << "Failed to load background image!"; success = false;
    }
    if(!gTKleeSpriteSheet.loadFromFile("image/characters/Heroes/Klee.png", gRenderer)){
        cout << "Failed to load character image!"; success = false;
    }
    if(!gTMap.loadFromFile("image/png/Map/map_1-1.png", gRenderer)){
        cout << "Failed to load map!"; success = false;
    }
    else{
        //Set sprite clips
        gKleeSpriteClips[0].x = 0;
        gKleeSpriteClips[0].y = 97;
        gKleeSpriteClips[0].w = 32.5;
        gKleeSpriteClips[0].h = 48.5;

        gKleeSpriteClips[1].x = 0;
        gKleeSpriteClips[1].y = 48.5;
        gKleeSpriteClips[1].w = 32.5;
        gKleeSpriteClips[1].h = 48.5;
/*
        gKleeSpriteClips[2].x = 65;
        gKleeSpriteClips[2].y = 97;
        gKleeSpriteClips[2].w = 32.5;
        gKleeSpriteClips[2].h = 48.5;

        gKleeSpriteClips[3].x = 97.5;
        gKleeSpriteClips[3].y = 97;
        gKleeSpriteClips[3].w = 32.5;
        gKleeSpriteClips[3].h = 48.5; */
    }
    return success;
}

void close(){
	//Free loaded image
	//gTButtonSprite.free();
	gTBackground.free();
	gTKlee.free();
	gTKleeSpriteSheet.free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL; gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


