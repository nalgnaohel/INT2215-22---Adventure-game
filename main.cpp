#include <iostream>
#include <string.h>
#include "Utils.h"
#include "LTexture.h"
#include "Klee.h"
//#include "Button.h"
//#include "Tile.h"
using namespace std;

#define pb push_back

const string WINDOW_TITLE = "Game ver 1.0";

SDL_Texture* loadTexture(string path);
bool initSDL();
bool loadMedia(Tile* tiles[]);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void close(Tile* tiles[]);
bool setTiles(Tile* tiles[]);

SDL_Window* gWindow = NULL;
LTexture gTBackground;
LTexture gTKlee;
LTexture gTMap;
LTexture gTKleeSpriteSheet;
SDL_Rect gKleeSpriteClips[KLEE_WALKING_FRAME];
SDL_Renderer* gRenderer = NULL;
SDL_Event event;
LTexture gTypeTiles[TOTAL_TILE_TYPES];
//Buttons
//Button gButtons[TOTAL_BUTTONS];

int main(int argc, char* argv[]){
    if(!initSDL()){
        cout << "Unable to initialize\n";
    }
    else{
        Tile* tiles[TOTAL_TILES];
        if(!loadMedia(tiles)){
            cout << "Unable to load media\n";
        }
        else{
            bool isRunning = 1; //double degree = 0;
            Klee klee;
            int frame = 0;
            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            int scrollingOffset = 0;
            //SDL_Rect wall; wall.x = 0; wall.y = LEVEL_HEIGHT - 64; wall.w = 32 * 10; wall.h = 64;
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
                klee.move(tiles);
                klee.setCamera(camera);
                cout << klee.getKleeBox().x << ' ' << klee.getKleeBox().y << ' ' << camera.x << ' ' << camera.y << ' ' << camera.w << ' ' << camera.h << '\n';
                --scrollingOffset;
                if(scrollingOffset < -gTBackground.getWidth()){
                    scrollingOffset = 0;
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

                gTBackground.render(scrollingOffset, 0, gRenderer);
                gTBackground.render(scrollingOffset + gTBackground.getWidth(), 0, gRenderer);
                for(int i = 0; i < TOTAL_TILES; i++){
                    if(tiles[i]->getType() != -1){
                        //cout << tiles[i]->getBox().x << ' ' << tiles[i]->getBox().y << ' ' << tiles[i]->getType() << '\n';
                        tiles[i]->render(gRenderer, gTypeTiles, camera);
                    }
                }
				//gTMap.render(0, 0, gRenderer, &camera);
				/*for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++){
                    gButtons[i].render(gTButtonSprite, gRenderer, NULL);
				}*/

				//Update screen
				SDL_Rect* currentClip = &gKleeSpriteClips[frame];
				klee.render(gTKleeSpriteSheet, gRenderer, currentClip, camera);
				//cout << klee.getKleePosX() << ' ' << klee.getKleePosY() << '\n';
				SDL_RenderPresent(gRenderer);
            }

        }
        close(tiles);
    }
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

bool loadMedia(Tile* tiles[]){
    bool success = true;
    if(!gTBackground.loadFromFile("image/png/BG/BG.png", gRenderer)){
        cout << "Failed to load background image!"; success = false;
    }
    if(!gTMap.loadFromFile("image/png/Map/map_1-1.png", gRenderer)){
        cout << "Failed to load map image!"; success = false;
    }
    if(!gTKleeSpriteSheet.loadFromFile("image/characters/Heroes/Klee.png", gRenderer)){
        cout << "Failed to load character!"; success = false;
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
    }
    for(int i = 1; i <= 18; i++){
        int tmp = i;
        string fpath = "image/png/Tiles/"; string id = to_string(tmp);
        fpath += id; fpath += ".png"; cout << fpath << '\n';
        if(!gTypeTiles[i].loadFromFile(fpath, gRenderer)){
            cout << "Failed to load tiles texture!"; success = false;
        }
    }
    if(!setTiles(tiles)){
        cout << "Failed to load map!\n"; success = false;
    }
    return success;
}

void close(Tile* tiles[]){
	//Free loaded image
	//gTButtonSprite.free();
	gTBackground.free();
	gTKlee.free();
	gTKleeSpriteSheet.free();
	for(int i = 0; i < TOTAL_TILE_TYPES; i++){
        gTypeTiles[i].free();
	}
    for(int i = 0; i < TOTAL_TILES; i++){
        if(tiles[i] != NULL){
            delete tiles[i];
            tiles[i] = NULL;
        }
    }
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL; gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool setTiles(Tile* tiles[]){
    bool success = true;
    //int x = 0, y = 0;
    fstream file("image/png/Map/map_1-1.csv");
    vector<string> map_row;
    vector<vector<string>> map_content;
    string line, stringId;
    if(file.is_open()){
        while(getline(file, line)){
            map_row.clear();
            stringstream str(line);
            while(getline(str, stringId, ',')){
                map_row.pb(stringId);
            }
            map_content.pb(map_row);
        }
    }
    int tileId = 0;
    for(int i = 0; i < (int)map_content.size(); i++){
        for(int j = 0; j < (int)map_content[i].size(); j++){
            string stringId = map_content[i][j];
            int intId = stoi(stringId); intId -= 14;
            if((intId < 1 || intId > 18) && intId != -15){
                cout << "Invalid tilemap\n"; success = false; break;
            }
            else{
                if(intId == -15) intId = -1;
                tiles[tileId] = new Tile(j * TILE_WIDTH, i * TILE_HEIGHT, intId); tileId++;
            }
            //cout << tileId << ' ' << intId << '\n';
        }
        //cout << '\n';
        if(success == false){
            break;
        }
    }
    return success;
}
