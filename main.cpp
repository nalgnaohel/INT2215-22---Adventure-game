#include "Utils.h"
#include "LTexture.h"
#include "Klee.h"
#include "Enemy.h"
#include "Buttons.h"
#include "Function.h"
#include "Timer.h"
#include "movingGround.h"

using namespace std;

const string WINDOW_TITLE = "Game ver 1.0";

bool initSDL();
bool loadMedia();
void close(Tile* tiles[]);
bool setTiles(Tile* tiles[], string mapName, vector<Enemy*> (&ghost)[TOTAL_ENEMIES]);

SDL_Window* gWindow = NULL;
LTexture gTBackground;
LTexture gTKlee[18];
LTexture gTKleeHb;
LTexture gTKleeHbBackground;
LTexture gTEnemy[TOTAL_ENEMIES][12];
LTexture gTArrow[6];
LTexture gTText;
LTexture gTMenuButtonsSpriteSheet[2];
LTexture gTNPC1;
LTexture gTEndLevelSign;
LTexture gTEndLevelDialog;
LTexture gTEndLevelText;
TTF_Font* gFont = NULL;
TTF_Font* gTitleFont = NULL;
LTexture gTTime;
Klee* klee = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gTypeTiles[TOTAL_TILE_TYPES];

vector<Buttons> menuStart;
string lv;
vector<Enemy*> ghost[TOTAL_ENEMIES];
vector<pair<pair<int, int>, int>> ground;
vector<int> mvGroundId;
vector<movingGround> upDown;
Tile* tiles[MAX_TOTAL_TILES];
int TOTAL_TILES;
SDL_Rect gKleeSpriteClips[18][16];
int gKleeSpriteClipsSize[18];
SDL_Rect gKleeHbClips[2];
SDL_Rect gArrowSpriteClips[6][9];
int gArrowSpriteClipsSize[6];
SDL_Rect gEnemySpriteClips[2][12][7];
int gEnemySpriteClipsSize[2][12];
int tot_tiles = 0;

string fileKlee = "image/characters/Wanderer Magican";
string fileGhost[2] = {"Onre", "Yurei"};
string fileKleeState[9] = {"Attack_1", "Dead", "Hurt", "Idle", "Jump", "Magic_arrow", "Magic_sphere", "Run", "Walk"};
string fileArrow[2] = {"Charge_1", "Charge_2"};
string fileGhost0[6] = {"Attack_2", "Dead", "Hurt", "Run", "Scream", "Walk"};
string fileGhost1[6] = {"Attack_3", "Dead", "Hurt", "Run", "Scream", "Walk"};

bool isGround(string typeTile){
    int id = stoi(typeTile);
    for(int j = 0; j < 9; j++){
        if(GROUND_ID[j] == id){
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]){
    if(!initSDL()){
        cout << "Unable to initialize\n";
    }
    else{
        if(!loadMedia()){
            cout << "Unable to load media\n";
        }
        else{
            bool isRunning = 1; bool play = 1;
            SDL_Color btmpColor = {121, 48, 48};

            Buttons btmp(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
            btmp.setPosition(SCREEN_WIDTH / 2 - MENU_BUTTON_WIDTH / 2, 350);
            btmp.setButtonName("Play!", btmpColor, gFont);
            Buttons btmp1(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
            btmp1.setPosition(btmp.getButtonBox().x, btmp.getButtonBox().y + MENU_BUTTON_HEIGHT + 50);
            btmp1.setButtonName("Bye...", btmpColor, gFont);
            menuStart.pb(btmp); menuStart.pb(btmp1);
            //show menu
            int gameState = showMenu(gTBackground, gTMenuButtonsSpriteSheet, gRenderer, menuStart);
            if(gameState){
                play = 0; isRunning = 0;
            }
            lv = "1-1";
            bool startLevel = 0;

            //run game
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            while(play){
                startLevel = 0;
                SDL_Event event;
                bool gameOver = 0;
                int scrollingOffset = 0;
                SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                ground.clear();
                upDown.clear();
                if(klee != NULL){
                    delete klee; klee = NULL;
                }
                for(int i = 0; i < MAX_TOTAL_TILES; i++){
                    if(tiles[i] != NULL){
                        delete tiles[i];
                        tiles[i] = NULL;
                    }
                }
                for(int i = 0; i < TOTAL_ENEMIES; i++){
                    for(int j = 0; j < (int)ghost[i].size(); j++){
                        if(ghost[i][j] != NULL){
                            Enemy* e = ghost[i].at(j);
                            if(e != NULL){
                                ghost[i].erase(ghost[i].begin() + j);
                                delete e;
                                e = NULL;
                            }
                        }
                    }
                }
                klee = new Klee(gKleeSpriteClips[3][0]);
                bool died = 0;
                Timer fps_timer;
                while(isRunning){
                    fps_timer.start();
                    if(!startLevel){
                        if(!setTiles(tiles, lv, ghost)){
                            play = 0; isRunning = 0;
                            cout << "Unable to load map!\n"; break;
                        }
                        else{
                            startLevel = 1; klee->TOTAL_TILES = tot_tiles;

                        }
                    }
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer);

                    gTBackground.render(0, 0, gRenderer);
                    for(int i = 0; i < tot_tiles; i++){
                        if(tiles[i]->getType() != -1){
                            tiles[i]->render(gRenderer, gTypeTiles, camera);
                        }
                    }
                    while(SDL_PollEvent(&event)){
                        if(event.type == SDL_QUIT){
                            isRunning = 0;
                        }
                        klee->handleKleeEvent(event, gKleeSpriteClips, ground);
                    }
                    for(int i = 0; i < (int)upDown.size(); i++){
                        upDown[i].move(tiles, klee);
                        ground[mvGroundId[i]].fi.fi = upDown[i].getLx();
                        ground[mvGroundId[i]].fi.se = upDown[i].getRx();
                        ground[mvGroundId[i]].se = upDown[i].getBox().y;
                    }

                    //cout << 3 << '\n';
                    klee->handleArrowList(gRenderer, gTArrow, gArrowSpriteClips, gArrowSpriteClipsSize, camera, ghost, gEnemySpriteClips);
                    klee->move(tiles, ground, gKleeSpriteClips);
                    klee->setCamera(camera);
                    if(!died){
                        for(int i = 0; i < TOTAL_ENEMIES; i++){
                            for(int j = 0; j < (int)ghost[i].size(); j++){
                                //ghost[i][j]->updateHealth(klee->damage[i][j], gEnemySpriteClips);
                                SDL_Rect kBox = klee->getKleeBox();
                                ghost[i][j]->action(kBox, gEnemySpriteClips, gEnemySpriteClipsSize);
                                klee->updateKleeHealth(ghost[i][j]->getEnemyDamage());
                                if(klee->getKleeHealth() <= 0){
                                    died = 1; klee->spriteId = 1; klee->kleeFrame = 0; break;
                                }
                                if(klee->spriteId % 9 == 0 && klee->kleeFrame % KLEE_ID_FRAME[klee->spriteId % 9] == 0 && klee->kleeFrame >= 3 * KLEE_ID_FRAME[klee->spriteId % 9]){
                                    cout << "Klee attack 1\n";
                                    SDL_Rect dam;
                                    if(klee->spriteId == 0){
                                        dam = {klee->getKleeBox().x, klee->getKleeBox().y - 36, klee->getKleeBox().w + 36, klee->getKleeBox().h + 72};
                                    }
                                    else{
                                        dam = {klee->getKleeBox().x - 36, klee->getKleeBox().y - 36, klee->getKleeBox().w + 36, klee->getKleeBox().h + 72};
                                    }
                                    SDL_Rect eBox = ghost[i][j]->getEnemyBox();
                                    if(checkCollision(dam, eBox)){
                                        cout << "Hit! - " << i << ' ' << j << '\n';
                                        ghost[i][j]->updateHealth(10, gEnemySpriteClips);
                                    }
                                }
                                int fr = ghost[i][j]->frame;
                                if(fr == gEnemySpriteClipsSize[ghost[i][j]->type][1] * GHOST_ID_FRAME[ghost[i][j]->type][1] - 1){
                                    //delete this ghost
                                    Enemy* e = ghost[i].at(j);
                                    ghost[i].erase(ghost[i].begin() + j);
                                    delete e;
                                    e = NULL;
                                }
                            }
                            if(died){
                                break;
                            }
                        }
                    }
                    //cout << klee->onGround(ground) << ' ' << klee->spriteId << ' ' << klee->kleeFrame << ' ' << klee->getKleeHealth() << ' ';
                    for(int i = 0; i < TOTAL_ENEMIES; i++){
                        for(int j = 0; j < (int)ghost[i].size(); j++){
                            //cout << ghost[i][j]->getHealth() << ' ';
                        }
                    }
                    //cout << '\n';
                    if(died && klee->kleeFrame == gKleeSpriteClipsSize[1] * KLEE_ID_FRAME[1] - 1){
                        gameOver = true;
                    }
                    if(klee->getKleeBox().y > SCREEN_HEIGHT || klee->getKleeBox().x + klee->getKleeBox().w >= LEVEL_WIDTH){
                        gameOver = true;
                    }
                    if(gameOver){
                        isRunning = false;
                    }

                    //cout << klee->spriteId << ' ' << klee->kleeFrame << ' ' << klee->onGround(ground) << '\n';
                    SDL_Rect* currentClip;
                    currentClip = &gKleeSpriteClips[klee->spriteId][klee->kleeFrame / KLEE_ID_FRAME[klee->spriteId % 9]];
                    klee->render(gTKlee[klee->spriteId], gTKleeHb, gTKleeHbBackground, gKleeHbClips, gRenderer, currentClip, camera);
                    for(int i = 0; i < TOTAL_ENEMIES; i++){
                        for(int j = 0; j < (int)ghost[i].size(); j++){
                            int fr = ghost[i][j]->frame; int sp = ghost[i][j]->spriteId;
                            //cout << i << ' ' << j << ": " << fr << ' ' << sp << ' ' << ghost[i][j]->dir << '\n' ;
                            SDL_Rect* clip = &gEnemySpriteClips[i][sp][fr / GHOST_ID_FRAME[i][sp % 6]];
                            ghost[i][j]->render(gTEnemy[i][sp], gRenderer, clip, camera);
                        }
                    }
                    //gTEnemy[0][11].render(30, 30, gRenderer, &gEnemySpriteClips[0][11][0]);
                    int now_time = fps_timer.getCurTicks();
                    int elasped = SDL_GetTicks();
                    string time_ = "Time: " + to_string(elasped);
                    SDL_Color titleColor = {255, 255, 255};
                    if(!gTTime.loadFromRenderedText(time_, titleColor, gRenderer, gFont)){
                        cout << "Unable to display time\n"; break;
                    }
                    else{
                        gTTime.render(10, 10, gRenderer);
                    }
                    int frameTime = 1000 / FRAME_PER_SECOND;
                    //cout << now_time << ' ' << frameTime << '\n';
                    if(now_time < frameTime){
                        int dl = frameTime - now_time;
                        SDL_Delay(dl);
                    }
                    SDL_RenderPresent(gRenderer);
                    //update frame
                    //Klee first
                    klee->updateFrame(gKleeSpriteClips, gKleeSpriteClipsSize, ground, gArrowSpriteClips, camera);
                    //Now the enemies
                    for(int i = 0; i < TOTAL_ENEMIES; i++){
                        for(int j = 0; j < (int)ghost[i].size(); j++){
                            ghost[i][j]->updateFrame(gEnemySpriteClips, gEnemySpriteClipsSize);
                        }
                    }
                    klee->s_cd = max(0, klee->s_cd - 1);
                }
                if(gameOver){
                    fps_timer.stop();
                    int gameState = showGameOver("Game Over!", gTBackground, gTEndLevelDialog, gTEndLevelText, gTMenuButtonsSpriteSheet, gRenderer, gFont, menuStart);
                    if(gameState == 1){
                        SDL_RenderClear(gRenderer);
                        int s = showMenu(gTBackground, gTMenuButtonsSpriteSheet, gRenderer, menuStart);
                        if(s){
                            isRunning = 0; play = 0;
                        }
                        if(!s){
                            isRunning = 1; gameOver = 0; scrollingOffset = 0;
                        }
                    }
                    else{
                        isRunning = 1; gameOver = 0; scrollingOffset = 0;
                    }
                }
            }
        }
        close(tiles);
        TTF_CloseFont(gFont);
        //Destroy window
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gRenderer = NULL; gWindow = NULL; gFont = NULL;

        //Quit SDL subsystems
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
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
                    cout << "Could not initialize SDL_Image! SDL_image Error: " << IMG_GetError() << '\n';
                    success = false;
                }

                //init TTF
                if(TTF_Init() == -1){
                    cout << "Could not initialize SDL_TTF! SDL_ttf Error: " << TTF_GetError() << '\n';
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia(){
    bool success = true;
    if(!gTBackground.loadFromFile("image/png/BG/background_2.jpg", gRenderer)){
        cout << "Failed to load background image!"; success = false;
    }
    if(!gTEndLevelSign.loadFromFile("image/png/Object/Sign_1.png", gRenderer)){
        cout << "Failed to load sign!"; success = false;
    }
    for(int i = 0; i < 18; i++){
        string tmp;
        if(i >= 9){
            tmp = fileKleeState[i - 9] + "_f";
        }
        else{
            tmp = fileKleeState[i];
        }
        string fname = fileKlee + "/" + tmp + ".png";
        cout << i << ": " << fname << '\n';
        if(!gTKlee[i].loadFromFile(fname, gRenderer)){
            cout << "Failed to load character's " << fileKleeState[i] << " state\n"; success = false;
        }
        //cout << '\n';
    }
    if(success){
        gKleeSpriteClips[0][0] = {31, 64, 41, 64}; gKleeSpriteClips[9][0] = {824, 64, 41, 64};
        gKleeSpriteClips[0][1] = {160, 65, 38, 63}; gKleeSpriteClips[9][1] = {698, 65, 38, 63};
        gKleeSpriteClips[0][2] = {282, 62, 68, 66}; gKleeSpriteClips[9][2] = {545, 62, 68, 66};
        gKleeSpriteClips[0][3] = {405, 45, 84, 83}; gKleeSpriteClips[9][3] = {406, 45, 84, 83};
        gKleeSpriteClips[0][4] = {532, 41, 90, 87}; gKleeSpriteClips[9][4] = {274, 41, 90, 87};
        gKleeSpriteClips[0][5] = {661, 29, 99, 99}; gKleeSpriteClips[9][5] = {136, 29, 99, 99};
        gKleeSpriteClips[0][6] = {795, 62, 72, 66}; gKleeSpriteClips[9][6] = {31, 62, 72, 66};
        gKleeSpriteClipsSize[0] = 7; gKleeSpriteClipsSize[9] = 7;

        gKleeSpriteClips[2][0] = {41, 69, 41, 59}; gKleeSpriteClips[11][0] = {429, 69, 42, 59};
        gKleeSpriteClips[2][1] = {166, 69, 40, 59}; gKleeSpriteClips[11][1] = {303, 69, 40, 59};
        gKleeSpriteClips[2][2] = {295, 69, 39, 59}; gKleeSpriteClips[11][2] = {177, 69, 39, 59};
        gKleeSpriteClips[2][3] = {425, 69, 42, 59}; gKleeSpriteClips[11][3] = {44, 69, 42, 59};
        gKleeSpriteClipsSize[2] = 4; gKleeSpriteClipsSize[11] = 4;

        gKleeSpriteClips[1][0] = {32, 69, 39, 59}; gKleeSpriteClips[10][0] = {440, 69, 39, 59};
        gKleeSpriteClips[1][1] = {159, 72, 40, 56}; gKleeSpriteClips[10][1] = {314, 72, 40, 56};
        gKleeSpriteClips[1][2] = {289, 88, 58, 40};  gKleeSpriteClips[10][2] = {166, 88, 58, 40};
        gKleeSpriteClips[1][3] = {416, 118, 62, 10}; gKleeSpriteClips[10][3] = {34, 118, 62, 10};
        gKleeSpriteClipsSize[1] = 4; gKleeSpriteClipsSize[10] = 4;

        gKleeSpriteClips[3][0] = {46, 62, 35, 66}; gKleeSpriteClips[12][0] = {943, 62, 35, 66};
        gKleeSpriteClipsSize[3] = 1; gKleeSpriteClipsSize[12] = 1;

        gKleeSpriteClips[4][0] = {19, 68, 59, 60}; gKleeSpriteClips[13][0] = {945, 68, 59, 60};
        gKleeSpriteClips[4][1] = {166, 72, 35, 56}; gKleeSpriteClips[13][1] = {823, 72, 35, 56};
        gKleeSpriteClips[4][2] = {292, 74, 39, 54}; gKleeSpriteClips[13][2] = {690, 74, 39, 54};
        gKleeSpriteClips[4][3] = {428, 57, 41, 63}; gKleeSpriteClips[13][3] = {555, 57, 41, 63};
        gKleeSpriteClips[4][4] = {550, 45, 49, 63}; gKleeSpriteClips[13][4] = {425, 45, 49, 63};
        gKleeSpriteClips[4][5] = {678, 53, 67, 72}; gKleeSpriteClips[13][5] = {280, 53, 67, 72};
        gKleeSpriteClips[4][6] = {798, 71, 60, 57}; gKleeSpriteClips[13][6] = {167, 71, 60, 57};
        gKleeSpriteClips[4][7] = {940, 72, 34, 56}; gKleeSpriteClips[13][7] = {51, 72, 34, 56};
        gKleeSpriteClipsSize[4] = 8; gKleeSpriteClipsSize[13] = 8;

        gKleeSpriteClips[5][0] = {37, 65, 39, 63}; gKleeSpriteClips[14][0] = {692, 65, 39, 63};
        gKleeSpriteClips[5][1] = {163, 65, 47, 63}; gKleeSpriteClips[14][1] = {558, 65, 47, 63};
        gKleeSpriteClips[5][2] = {293, 63, 55, 65}; gKleeSpriteClips[14][2] = {420, 63, 55, 65};
        gKleeSpriteClips[5][3] = {420, 63, 55, 65}; gKleeSpriteClips[14][3] = {292, 63, 55, 65};
        gKleeSpriteClips[5][4] = {550, 65, 54, 63}; gKleeSpriteClips[14][4] = {163, 63, 54, 65};
        gKleeSpriteClips[5][5] = {678, 66, 54, 62}; gKleeSpriteClips[14][5] = {37, 63, 54, 65};
        gKleeSpriteClipsSize[5] = 6; gKleeSpriteClipsSize[14] = 6;

        gKleeSpriteClips[6][0] = {32, 65, 40, 63}; gKleeSpriteClips[15][0] = {1976, 65, 40, 63};
        gKleeSpriteClips[6][1] = {163, 65, 44, 63}; gKleeSpriteClips[15][1] = {1841, 65, 44, 63};
        gKleeSpriteClips[6][2] = {293, 63, 60, 65}; gKleeSpriteClips[15][2] = {1695, 63, 60, 65};
        gKleeSpriteClips[6][3] = {422, 63, 60, 65}; gKleeSpriteClips[15][3] = {1567, 63, 60, 65};
        gKleeSpriteClips[6][4] = {549, 63, 61, 65}; gKleeSpriteClips[15][4] = {1438, 63, 61, 65};
        gKleeSpriteClips[6][5] = {667, 63, 62, 65}; gKleeSpriteClips[15][5] = {1309, 63, 62, 65};
        gKleeSpriteClips[6][6] = {805, 63, 63, 65}; gKleeSpriteClips[15][6] = {1180, 63, 63, 65};
        gKleeSpriteClips[6][7] = {933, 63, 58, 65}; gKleeSpriteClips[15][7] = {1064, 63, 58, 65};
        gKleeSpriteClips[6][8] = {1061, 63, 58, 65}; gKleeSpriteClips[15][8] = {929, 63, 58, 65};
        gKleeSpriteClips[6][9] = {1189, 63, 60, 65}; gKleeSpriteClips[15][9] = {799, 63, 60, 65};
        gKleeSpriteClips[6][10] = {1317, 63, 61, 65}; gKleeSpriteClips[15][10] = {670, 63, 61, 65};
        gKleeSpriteClips[6][11] = {1446, 64, 60, 64}; gKleeSpriteClips[15][11] = {542, 64, 60, 64};
        gKleeSpriteClips[6][12] = {1564, 64, 75, 64}; gKleeSpriteClips[15][12] = {409, 64, 75, 64};
        gKleeSpriteClips[6][13] = {1688, 64, 67, 64}; gKleeSpriteClips[15][13] = {293, 64, 67, 64};
        gKleeSpriteClips[6][14] = {1816, 64, 67, 64}; gKleeSpriteClips[15][14] = {165, 64, 67, 64};
        gKleeSpriteClips[6][15] = {1954, 64, 57, 64}; gKleeSpriteClips[15][15] = {37, 64, 57, 64};
        gKleeSpriteClipsSize[6] = 16; gKleeSpriteClipsSize[15] = 16;

        //gotta calculate this state again
        gKleeSpriteClips[7][0] = {23, 67, 44, 61}; gKleeSpriteClips[16][0] = {957, 67, 44, 61};
        gKleeSpriteClips[7][1] = {143, 66, 54, 62}; gKleeSpriteClips[16][1] = {827, 66, 54, 62};
        gKleeSpriteClips[7][2] = {265, 68, 64, 60}; gKleeSpriteClips[16][2] = {695, 68, 64, 60};
        gKleeSpriteClips[7][3] = {389, 65, 65, 63}; gKleeSpriteClips[16][3] = {570, 65, 65, 63};
        gKleeSpriteClips[7][4] = {530, 65, 50, 63}; gKleeSpriteClips[16][4] = {444, 65, 50, 63};
        gKleeSpriteClips[7][5] = {669, 66, 37, 62}; gKleeSpriteClips[16][5] = {319, 66, 37, 62};
        gKleeSpriteClips[7][6] = {796, 68, 45, 60}; gKleeSpriteClips[16][6] = {184, 68, 45, 60};
        gKleeSpriteClips[7][7] = {919, 67, 43, 61}; gKleeSpriteClips[16][7] = {63, 67, 43, 61};
        gKleeSpriteClipsSize[7] = 8; gKleeSpriteClipsSize[16] = 8;

        gKleeSpriteClips[8][0] = {49, 62, 31, 66}; gKleeSpriteClips[17][0] = {816, 62, 31, 66};
        gKleeSpriteClips[8][1] = {172, 62, 46, 66}; gKleeSpriteClips[17][1] = {678, 62, 46, 66};
        gKleeSpriteClips[8][2] = {289, 62, 43, 66}; gKleeSpriteClips[17][2] = {544, 62, 43, 66};
        gKleeSpriteClips[8][3] = {422, 62, 42, 66}; gKleeSpriteClips[17][3] = {432, 62, 42, 66};
        gKleeSpriteClips[8][4] = {554, 62, 35, 66}; gKleeSpriteClips[17][4] = {307, 62, 35, 66};
        gKleeSpriteClips[8][5] = {688, 62, 26, 66}; gKleeSpriteClips[17][5] = {182, 62, 26, 66};
        gKleeSpriteClips[8][6] = {817, 62, 29, 66}; gKleeSpriteClips[17][6] = {50, 62, 29, 66};
        gKleeSpriteClipsSize[8] = 7; gKleeSpriteClipsSize[17] = 7;
    }
    if(!gTKleeHbBackground.loadFromFile("image/characters/Healthbar/Healthbar DARK.png", gRenderer)){
        cout << "Failed to render healthbar background!\n"; success = false;
    }
    if(!gTKleeHb.loadFromFile("image/characters/Healthbar/Healthbar DARK.png", gRenderer)){
        cout << "Failed to render healthbar background!\n"; success = false;
    }
    if(success){
        gKleeHbClips[0] = {9, 94, 76, 18}; //background
        gKleeHbClips[1] = {28, 90, 56, 4};
    }
    for(int i = 0; i < 6; i++){
        string tmp;
        if(i == 2 || i == 5){
            continue;
        }
        if(i < 2){
            tmp = fileArrow[i];
        }
        else{
            tmp = fileArrow[i - 3] + "_f";
        }
        string fname = fileKlee + "/" + tmp + ".png";
        //cout << i << ": " << fname << '\t';
        if(!gTArrow[i].loadFromFile(fname, gRenderer)){
            cout << "Failed to load arrow!\n"; success = false;
        }
    }
    if(success){
        gArrowSpriteClips[0][0] = {26, 54, 30, 18}; gArrowSpriteClips[3][0] = {520, 54, 30, 18};
        gArrowSpriteClips[0][1] = {90, 54, 30, 18}; gArrowSpriteClips[3][1] = {456, 54, 30, 18};
        gArrowSpriteClips[0][2] = {152, 56, 32, 14}; gArrowSpriteClips[3][2] = {392, 56, 32, 14};
        gArrowSpriteClips[0][3] = {211, 56, 37, 15}; gArrowSpriteClips[3][3] = {328, 56, 37, 15};
        gArrowSpriteClips[0][4] = {278, 56, 34, 15}; gArrowSpriteClips[3][4] = {264, 56, 34, 15};
        gArrowSpriteClips[0][5] = {412, 46, 27, 42}; gArrowSpriteClips[3][5] = {200, 46, 27, 42};
        gArrowSpriteClips[0][6] = {462, 42, 31, 46}; gArrowSpriteClips[3][6] = {83, 42, 31, 46};
        gArrowSpriteClips[0][7] = {510, 47, 43, 39}; gArrowSpriteClips[3][7] = {23, 47, 43, 49};
        gArrowSpriteClipsSize[0] = 8; gArrowSpriteClipsSize[3] = 8;

        gArrowSpriteClips[1][0] = {96, 61, 16, 6}; gArrowSpriteClips[4][0] = {272, 61, 16, 6};
        gArrowSpriteClips[1][1] = {154, 61, 23, 5}; gArrowSpriteClips[4][1] = {207, 61, 23, 5};
        gArrowSpriteClips[1][2] = {214, 61, 27, 5}; gArrowSpriteClips[4][2] = {143, 61, 27, 5};
        gArrowSpriteClips[1][3] = {278, 61, 27, 5}; gArrowSpriteClips[4][3] = {79, 61, 27, 5};
        gArrowSpriteClips[1][4] = {341, 61, 28, 5}; gArrowSpriteClips[4][4] = {15, 61, 28, 5};
        gArrowSpriteClipsSize[1] = 5; gArrowSpriteClipsSize[4] = 5;
    }
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        for(int j = 0; j < 12; j++){
            string tmp;
            if(j < 6){
                if(i == 0){
                    tmp = fileGhost0[j];
                }
                else{
                    tmp = fileGhost1[j];
                }
            }
            else{
                if(i == 0) tmp = fileGhost0[j - 6] + "_f";
                else tmp = fileGhost1[j - 6] + "_f";
            }
            string fname = "C:/Users/HP/Documents/code/ltnc/gamebtl/image/characters/" + fileGhost[i] + "/" + tmp + ".png";
            cout << i << ' ' << j << ": " << fname << '\n';
            if(!gTEnemy[i][j].loadFromFile(fname, gRenderer)){
                cout << "Failed to load " << fileGhost[i] << "'s " << tmp << " state\n"; success = false;
            }
        }
    }
    if(success){
        gEnemySpriteClips[0][0][0].w = 48;
        gEnemySpriteClips[0][0][1].w = 30;
        gEnemySpriteClips[0][0][2].w = 50;
        gEnemySpriteClips[0][0][3].w = 42;
        for(int i = 0; i < 4; i++){
            gEnemySpriteClips[0][0][i].x = i * 127 + 55;
            gEnemySpriteClips[0][0][i].y = 57;
            gEnemySpriteClips[0][0][i].h = 71;
        }
        gEnemySpriteClipsSize[0][0] = 4;

        for(int i = 0; i < 6; i++){
            gEnemySpriteClips[0][1][i].x = i * 128 + 42;
            gEnemySpriteClips[0][1][i].w = 47;
            gEnemySpriteClips[0][1][i].h = 68;
            gEnemySpriteClips[0][1][i].y = 57;
        }
        gEnemySpriteClipsSize[0][1] = 6;

        for(int i = 0; i < 3; i++){
            gEnemySpriteClips[0][2][i].x = i * 125 + 46;
            gEnemySpriteClips[0][2][i].y = 57;
            gEnemySpriteClips[0][2][i].w = 34;
            gEnemySpriteClips[0][2][i].h = 68;
        }
        gEnemySpriteClipsSize[0][2] = 3;

        for(int i = 0; i < 7; i++){
            gEnemySpriteClips[0][3][i].x = i * 128 + 35; gEnemySpriteClips[0][4][i].x = i * 128 + 53;
            gEnemySpriteClips[0][3][i].y = 59; gEnemySpriteClips[0][4][i].y = 59;
            gEnemySpriteClips[0][3][i].h = 69; gEnemySpriteClips[0][4][i].h = 71;
            gEnemySpriteClips[0][3][i].w = 40; gEnemySpriteClips[0][4][i].w = 28;
            if(i == 6) gEnemySpriteClips[0][3][i].w += 4;

            gEnemySpriteClips[0][5][i].x = i * 127 + 54;
            gEnemySpriteClips[0][5][i].y = 57;
            gEnemySpriteClips[0][5][i].w = 24;
            gEnemySpriteClips[0][5][i].h = 71;
        }
        for(int i = 3; i <= 5; i++){
            gEnemySpriteClipsSize[0][i] = 7;
        }

        gEnemySpriteClips[1][0][0].w = 56;
        gEnemySpriteClips[1][0][1].w = 64;
        gEnemySpriteClips[1][0][2].w = 65;
        gEnemySpriteClips[1][0][3].w = 72;
        gEnemySpriteClips[1][0][4].w = 61;
        gEnemySpriteClips[1][0][5].w = 61;
        gEnemySpriteClips[1][0][6].w = 56;
        for(int i = 0; i < 7; i++){
            gEnemySpriteClips[1][0][i].x = i * 125 + 35;
            gEnemySpriteClips[1][0][i].y = 56;
            gEnemySpriteClips[1][0][i].h = 71;
        }
        gEnemySpriteClipsSize[1][0] = 7;

        for(int i = 0; i < 7; i++){
            gEnemySpriteClips[1][1][i].x = i * 128 + 35;
            gEnemySpriteClips[1][1][i].y = 58;
            gEnemySpriteClips[1][1][i].h = 70;
            gEnemySpriteClips[1][1][i].w = 60;
        }
        gEnemySpriteClipsSize[1][1] = 7;

        gEnemySpriteClips[1][2][0].w = 53; gEnemySpriteClips[1][2][1].w = 60; gEnemySpriteClips[1][2][2].w = 58;
        for(int i = 0; i < 3; i++){
            gEnemySpriteClips[1][2][i].y = 57;
            gEnemySpriteClips[1][2][i].h = 69;
            gEnemySpriteClips[1][2][i].x = i * 124 + 40;
        }
        gEnemySpriteClipsSize[1][2] = 3;

        for(int i = 0; i < 5; i++){
            gEnemySpriteClips[1][3][i].y = 49;
            gEnemySpriteClips[1][3][i].h = 71;
            gEnemySpriteClips[1][3][i].x = i * 127 + 33;
            gEnemySpriteClips[1][3][i].w = 58;
        }
        gEnemySpriteClipsSize[1][3] = 5;

        for(int i = 0; i < 4; i++){
            gEnemySpriteClips[1][4][i].y = 54;
            gEnemySpriteClips[1][4][i].h = 70;
            gEnemySpriteClips[1][4][i].w = 58;
            gEnemySpriteClips[1][4][i].x = i * 125 + 33;
        }
        gEnemySpriteClipsSize[1][4] = 4;

        for(int i = 0; i < 5; i++){
            gEnemySpriteClips[1][5][i].y = 53;
            gEnemySpriteClips[1][5][i].h = 70;
            gEnemySpriteClips[1][5][i].x = i * 127 + 33;
            gEnemySpriteClips[1][5][i].w = 38;
        }
        gEnemySpriteClipsSize[1][5] = 5;

        for(int i = 0; i < TOTAL_ENEMIES; i++){
            for(int j = 6; j < 12; j++){
                for(int k = 0; k < gEnemySpriteClipsSize[i][j - 6]; k++){
                    gEnemySpriteClips[i][j][k].x = gTEnemy[i][j - 6].getWidth() - gEnemySpriteClips[i][j - 6][k].x - gEnemySpriteClips[i][j - 6][k].w;
                    gEnemySpriteClips[i][j][k].w = gEnemySpriteClips[i][j - 6][k].w;
                    gEnemySpriteClips[i][j][k].h = gEnemySpriteClips[i][j - 6][k].h;
                    gEnemySpriteClips[i][j][k].y = gEnemySpriteClips[i][j - 6][k].y;
                }
                gEnemySpriteClipsSize[i][j] = gEnemySpriteClipsSize[i][j - 6];
            }
        }
    }
    string tmp = fileArrow[0];
    string fname = "image/characters/Yurei/" + tmp + ".png"; cout << fname << '\n';
    if(!gTArrow[2].loadFromFile(fname, gRenderer)){
            cout << "Failed to load ghost's fire!\n"; success = false;
    }
    tmp += "_f";
    fname = "image/characters/Yurei/" + tmp + ".png"; cout << fname << '\n';
    if(!gTArrow[5].loadFromFile(fname, gRenderer)){
            cout << "Failed to load ghost's fire!\n"; success = false;
    }
    if(success){
        for(int i = 0; i <= 2; i++){
            gArrowSpriteClips[2][i].y = 8; gArrowSpriteClips[5][i].y = 8;
            gArrowSpriteClips[2][i].h = 8; gArrowSpriteClips[5][i].h = 8;
            gArrowSpriteClips[2][i].x = i * 24 + 5; gArrowSpriteClips[5][i].x = gTArrow[2].getWidth() - gArrowSpriteClips[2][i].x - gArrowSpriteClips[2][i].w;
            gArrowSpriteClips[2][i].w = 14; gArrowSpriteClips[5][i].w = 14;
        }
        gArrowSpriteClipsSize[2] = 3; gArrowSpriteClipsSize[5] = 3;
    }
    gTitleFont = TTF_OpenFont("fonts/Helmet.ttf", 60);
    if(gTitleFont == NULL){
        cout << "Unable to load Helmet font! " << TTF_GetError() << '\n'; success = false;
    }
    gFont = TTF_OpenFont("fonts/Mgen+ 1pp regular.ttf", 24);
    if(gFont == NULL){
        cout << "Unable to load Mgen regular font! " << TTF_GetError() << '\n'; success = false;
    }
    for(int i = 1; i <= 18; i++){
        int tmp = i;
        string fpath = "image/png/Tiles/"; string id = to_string(tmp);
        fpath += id; fpath += ".png"; cout << fpath << '\n';
        if(!gTypeTiles[i].loadFromFile(fpath, gRenderer)){
            cout << "Failed to load tiles texture!"; success = false;
        }
    }
    if(!gTMenuButtonsSpriteSheet[0].loadFromFile("image/paper/paper-button-off.png", gRenderer)){
        cout << "Failed to load on button!\n"; success = false;
    }
    if(!gTMenuButtonsSpriteSheet[1].loadFromFile("image/paper/paper-button-on.png", gRenderer)){
        cout << "Failed to load off button!\n"; success = false;
    }
    if(!gTEndLevelDialog.loadFromFile("image/paper/paper-dialog.png", gRenderer)){
        cout << "Failed to load dialog!\n"; success = false;
    }
    return success;
}

bool setTiles(Tile* tiles[], string mapName, vector<Enemy*> (&ghost)[TOTAL_ENEMIES]){
    bool success = true;
    string fmap = "image/png/Map/map_" + mapName + ".csv";
    fstream file(fmap);
    vector<string> map_row;
    vector<vector<string>> map_content;
    string line, stringId;
    vector<pair<pair<int, int>, pair<int, int>>> coor;
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
    file.close();
    int tileId = 0;int li = -1; int lj = -1;
    ground.pb(make_pair(make_pair(0, 9 * 32), 18 * 32));
    for(int i = 0; i < (int)map_content.size(); i++){
        for(int j = 0; j < (int)map_content[i].size(); j++){
            stringId = map_content[i][j];
            int intId = stoi(stringId); intId -= 14;
            if((intId < 1 || intId > 18) && intId != -15){
                cout << "Invalid tilemap\n"; success = false; break;
            }
            else{
                if(intId == -15) intId = -1;
                if(intId == 11){
                    li = i; lj = j;
                }
                if(intId == 7){
                    ground.pb(make_pair(make_pair(lj * TILE_WIDTH, j * TILE_WIDTH), i * TILE_HEIGHT));
                    coor.pb(make_pair(make_pair(lj, li), make_pair(j, i)));
                    li = -1; lj = -1;
                }
                if(intId == 1){
                    li = i; lj = j;
                }
                if(intId == 3){
                    ground.pb(make_pair(make_pair(lj * TILE_WIDTH, j * TILE_WIDTH), i * TILE_HEIGHT));
                    li = -1; lj = -1;
                }
                if(intId == 13){
                    li = i; lj = j;
                }
                if(intId == 15){
                    ground.pb(make_pair(make_pair(lj * TILE_WIDTH, j * TILE_WIDTH), i * TILE_HEIGHT));
                    if((int)upDown.size() < 3){
                        movingGround tmp(lj * TILE_WIDTH, (j + 1) * TILE_WIDTH, i * TILE_HEIGHT, lj * TILE_WIDTH, (j + 1) * TILE_WIDTH, 128, 576, "ud");
                        upDown.pb(tmp);
                        mvGroundId.pb((int)ground.size() - 1);
                    }
                }
                tiles[tileId] = new Tile(j * TILE_WIDTH, i * TILE_HEIGHT, intId); tileId++;
            }
            //cout << tileId << ' ' << intId << '\n';
        }
        //cout << '\n';
        if(success == false){
            break;
        }
    }
    for(int i = 0; i < (int)upDown.size(); i++){
        cout << upDown[i].getLT() << " - " << upDown[i].getly() << " - " << tiles[upDown[i].getLT()]->getBox().x << '\n';
        cout << upDown[i].getRT() << " - " << upDown[i].getry() << " - " << tiles[upDown[i].getRT()]->getBox().x << '\n';
    }
    ground.pb(make_pair(make_pair(102 * 32, 111 * 32), 18 * 32));
    tot_tiles = tileId;
    srand(time(NULL));
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        for(int j = 0; j < 1; j++){
            int id = rand() % (int)coor.size();
            int lx = coor[id].fi.fi; int y = coor[id].fi.se; int rx = coor[id].se.fi + 1;
            Enemy* e = new Enemy(lx * TILE_WIDTH, y * TILE_HEIGHT, rx * TILE_WIDTH, i, gEnemySpriteClips[i][5][0]);
            ghost[i].pb(e);
        }
    }
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        for(int j = 0; j < (int)ghost[i].size(); i++){
            cout << ghost[i][j]->getEnemyBox().x << ' ' << ghost[i][j]->getEnemyBox().y << '\n';
        }
    }
    //cout << "----------------------------------------------------------------------\n";
    //cout << "Ground:\n";
    return success;
}

void close(Tile* tiles[]){
	//Free loaded image
	gTBackground.free();
	for(int i = 0; i < 18; i++){
        gTKlee[i].free();
	}
	gTTime.free();
	gTText.free();
	gTEndLevelSign.free();
	gTMenuButtonsSpriteSheet[0].free();
	gTMenuButtonsSpriteSheet[1].free();
	for(int i = 0; i < 6; i++){
        gTArrow[i].free();
	}
	for(int i = 0; i < TOTAL_ENEMIES; i++){
        for(int j = 0; j < 12; j++){
            gTEnemy[i][j].free();
        }
	}
	for(int i = 0; i < TOTAL_TILE_TYPES; i++){
        gTypeTiles[i].free();
	}
	for(int i = 0; i < MAX_TOTAL_TILES; i++){
        if(tiles[i] != NULL){
            delete tiles[i];
            tiles[i] = NULL;
        }
	}
	delete klee;
	for(int i = 0; i < TOTAL_ENEMIES; i++){
        for(int j = 0; j < (int)ghost[i].size(); j++){
            if(ghost[i][j] != NULL){
                delete ghost[i][j];
                ghost[i][j] = NULL;
            }
        }
	}
	ground.clear();
	upDown.clear();
	menuStart.clear();
}
