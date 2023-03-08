#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 500;
const int SCREEN_BPP = 32;
const string WINDOW_TITLE = "Game ver 1.0";

SDL_Texture* loadTexture(string path);
bool initSDL();
bool loadMedia();
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void close();

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = NULL;
SDL_Texture* gTBackground = NULL;
SDL_Texture* gTKlee = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Event event;

int main(int argc, char* argv[]){
    if(!initSDL()){
        cout << "Unable to initialize\n";
    }
    else{
        if(!loadMedia()){
            cout << "Unable to load media\n";
        }
        else{
            bool isRunning = 1;
            while(isRunning){
                while(SDL_PollEvent(&event)){
                    if(event.type == SDL_QUIT){
                        isRunning = 0;
                    }
                }
                //Clear screen
				SDL_RenderClear(gRenderer);
				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTBackground, NULL, NULL );
				SDL_RenderCopy( gRenderer, gTKlee, NULL, NULL );
				SDL_RenderPresent( gRenderer );
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
        gWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(gWindow == NULL){
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else        {
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

void close(){
	//Free loaded image
	SDL_DestroyTexture(gTBackground);
	gTBackground = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL; gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia(){
    bool success = true;
    gTBackground = loadTexture("image/bg.png");
    if(gTBackground == NULL){
        cout << "Failed to load background texture image\n"; success = false;
    }
    gTKlee = loadTexture("image/klee_test.png");
    if(gTKlee == NULL){
        cout << "Failed to load character texture image\n"; success = false;
    }
    return success;
}

SDL_Texture* loadTexture(string path){
    //result texture
    SDL_Texture* resTexture = NULL;
    //Load image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        cout << "Unable to load image " << path.c_str() << "! SDL_Image Error: " << IMG_GetError() << '\n';
    }
    else{
        //create from surface pixels
        resTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(resTexture == NULL){
            cout << "Unable to create texture from surface " << path.c_str() << "! SDL Error: " << SDL_GetError() << '\n';
        }
        //Destroy old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return resTexture;
}
