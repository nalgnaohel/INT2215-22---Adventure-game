#include <iostream>
#include <string.h>
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
bool setTiles(Tile* tiles[], string mapName, vector<Enemy>& mush1);

SDL_Window* gWindow = NULL;
LTexture gTBackground;
LTexture gTKlee[9];
LTexture gTEnemy;
LTexture gTLightBall;
LTexture gTText;
LTexture gTMenuButtonsSpriteSheet[2];
LTexture gTNPC1;
LTexture gTEndLevelSign;
LTexture gTEndLevelDialog;
LTexture gTEndLevelText;
TTF_Font* gFont = NULL;
TTF_Font* gTitleFont = NULL;
LTexture gTTime;

SDL_Rect gLightBallSpriteClips[9];
SDL_Rect gNPC1SpriteClips[2];
SDL_Renderer* gRenderer = NULL;
LTexture gTypeTiles[TOTAL_TILE_TYPES];

vector<Buttons> menuStart;
string lv;
vector<Enemy> mush1;
vector<pair<pair<int, int>, int>> ground;
vector<int> mvGroundId;
vector<movingGround> upDown;
Tile* tiles[MAX_TOTAL_TILES];
int TOTAL_TILES;
SDL_Rect gKleeSpriteClips[9][12];
int gKleeSpriteClipsSize[9];
int tot_tiles = 0;

string fileKlee = "C:/Users/HP/Documents/code/ltnc/gamebtl/image/characters/Wanderer Magican";
string fileKleeState[9] = {"Attack_1", "Dead", "Hurt", "Idle", "Jump", "Magic_arrow", "Magic_sphere", "Run", "Walk" };
string fileLightBall = "Charge_1";

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
            Timer fps_timer; bool startLevel = 0;
            Klee klee(gKleeSpriteClips[3][0]);
            //run game
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            while(play){
                startLevel = 0;
                SDL_Event event;
                int numOfCollision = 0;
                bool gameOver = 0;
                int scrollingOffset = 0;
                SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                mush1.clear();
                ground.clear();
                upDown.clear();
                for(int i = 0; i < MAX_TOTAL_TILES; i++){
                    tiles[i] = NULL;
                }
                klee.reset(gKleeSpriteClips[3][0]);
                while(isRunning){
                    fps_timer.start();
                    if(!startLevel){
                        if(!setTiles(tiles, lv, mush1)){
                            play = 0; isRunning = 0;
                            cout << "Unable to load map!\n"; break;
                        }
                        else{
                            //cout << tot_tiles << '\n';
                            startLevel = 1; klee.TOTAL_TILES = tot_tiles;
                        }
                    }
                    while(SDL_PollEvent(&event)){
                        if(event.type == SDL_QUIT){
                            isRunning = 0;
                        }
                        klee.handleKleeEvent(event, gKleeSpriteClips, ground);
                    }
                    for(int i = 0; i < (int)upDown.size(); i++){
                        upDown[i].move(tiles);
                        ground[mvGroundId[i]].fi.fi = upDown[i].getLx();
                        ground[mvGroundId[i]].fi.se = upDown[i].getRx();
                        ground[mvGroundId[i]].se = upDown[i].getBox().y;
                    }
                    klee.move(tiles, ground, gKleeSpriteClips);
                    for(int i = 0; i < (int)mvGroundId.size(); i++){
                        cout << klee.getKleeBox().x << ' ' << klee.getKleeBox().x + klee.getKleeBox().w << ' ' << klee.getKleeBox().y << ' ' << klee.getKleeBox().y + klee.getKleeBox().h << " - ";
                        cout << ground[mvGroundId[i]].fi.fi << ' ' << ground[mvGroundId[i]].fi.se << ' ' << ground[mvGroundId[i]].se << '\n';
                    }
                    klee.setCamera(camera);
                    //cout << klee.spriteId << ' ' << klee.onGround(ground) << '\n';
                    for(int i = 0; i < (int)mush1.size(); i++){
                        mush1[i].move();
                    }
                    for(int i = 0; i < (int)mush1.size(); i++){
                        SDL_Rect kBox = klee.getKleeBox(); SDL_Rect m1Box = mush1[i].getEnemyBox();
                        if(checkCollision(kBox, m1Box)){
                            int damageTimes = 0;
                            numOfCollision++;
                            if(numOfCollision == 3){
                                if(damageTimes < 5){
                                    klee.updateKleeHealth(mush1[i].getEnemyDamage()); damageTimes++;
                                }
                                numOfCollision = 0;
                            }
                            cout << numOfCollision << ' ' << klee.getKleeHealth() << '\n';
                            if(klee.getKleeBox().x < m1Box.x){
                                mush1[i].dir = 0;
                            }
                            else{
                                mush1[i].dir = 1;
                            }
                            if(klee.getKleeLife() <= 0){
                                gameOver = 1;
                            }
                            if(gameOver) break;
                        }
                    }
                    if(klee.getKleeBox().y > SCREEN_HEIGHT){
                        gameOver = true;
                    }
                    if(gameOver){
                        isRunning = false;
                    }

                    //Clear screen
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer);

                    gTBackground.render(0, 0, gRenderer);
                    for(int i = 0; i < tot_tiles; i++){
                        //cout << i << ' ' << tiles[i]->getType() << '\n';
                        if(tiles[i]->getType() != -1){
                            tiles[i]->render(gRenderer, gTypeTiles, camera);
                        }
                    }
                    SDL_Rect* currentClip;
                    if(klee.spriteId == 0){
                        currentClip = &gKleeSpriteClips[klee.spriteId][klee.kleeFrame / KLEE_ID_FRAME[klee.spriteId]];
                    }
                    else{
                        currentClip = &gKleeSpriteClips[klee.spriteId][klee.kleeFrame / KLEE_ID_FRAME[klee.spriteId]];
                    }
                    klee.render(gTKlee[klee.spriteId], gRenderer, currentClip, camera);
                    for(int i = 0; i < (int)mush1.size(); i++){
                        mush1[i].render(gTEnemy, gRenderer, camera);
                    }
                    SDL_RenderPresent(gRenderer);
				//update frame
                    if(klee.spriteId != 4 && klee.spriteId != 0){
                        if(klee.spriteId == 3){
                            klee.kleeFrame = 0;
                        }
                        else{
                            klee.kleeFrame++;
                            if(klee.kleeFrame / KLEE_ID_FRAME[klee.spriteId] >= gKleeSpriteClipsSize[klee.spriteId]){
                                klee.kleeFrame = 0;
                            }
                        }
                        klee.updateKleeBox(gKleeSpriteClips[klee.spriteId][klee.kleeFrame / KLEE_ID_FRAME[klee.spriteId]], ground);
                    }
                    else if(klee.spriteId == 4) {
                        if(klee.kleeFrame >= 6 * KLEE_ID_FRAME[klee.spriteId]){
                            klee.kleeFrame++;
                            if(klee.kleeFrame == 8 * KLEE_ID_FRAME[klee.spriteId]){
                                klee.spriteId = 3; klee.kleeFrame = 0;
                            }
                        }
                        klee.updateKleeBox(gKleeSpriteClips[klee.spriteId][klee.kleeFrame / KLEE_ID_FRAME[klee.spriteId]], ground);
                    }
                    else{
                        klee.kleeFrame++;
                        if(klee.kleeFrame / KLEE_ID_FRAME[klee.spriteId] >= gKleeSpriteClipsSize[0]){
                            klee.kleeFrame = 0;
                        }
                        klee.updateKleeBox(gKleeSpriteClips[klee.spriteId][klee.kleeFrame / KLEE_ID_FRAME[klee.spriteId]], ground);
                    }
                    int now_time = fps_timer.getCurTicks();
                    int elasped = SDL_GetTicks();
                    string time_ = "Time: " + to_string(elasped);
                    SDL_Color titleColor = {51, 25, 0};
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
                }
                if(gameOver){
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
    for(int i = 0; i < 9; i++){
        string fname = fileKlee + "/" + fileKleeState[i] + ".png";
        if(!gTKlee[i].loadFromFile(fname, gRenderer)){
            cout << "Failed to load character's " << fileKleeState[i] << " state\n"; success = false;
        }
    }
    if(success){
        gKleeSpriteClips[0][0] = {23, 67, 78, 60};
        gKleeSpriteClips[0][1] = {150, 65, 78, 63};
        gKleeSpriteClips[0][2] = {256, 72, 108, 56};
        gKleeSpriteClips[0][3] = {400, 73, 91, 55};
        gKleeSpriteClipsSize[0] = 4;

        gKleeSpriteClips[2][0] = {40, 76, 32, 52};
        gKleeSpriteClips[2][1] = {169, 77, 32, 51};
        gKleeSpriteClips[2][2] = {296, 76, 32, 52};
        gKleeSpriteClipsSize[2] = 3;

        gKleeSpriteClips[1][0] = {36, 71, 34, 57};
        gKleeSpriteClips[1][1] = {164, 73, 33, 55};
        gKleeSpriteClips[1][2] = {293, 83, 23, 45};
        gKleeSpriteClips[1][3] = {423, 103, 36, 25};
        gKleeSpriteClips[1][4] = {551, 119, 53, 9};
        gKleeSpriteClipsSize[1] = 5;

        gKleeSpriteClips[3][0] = {43, 65, 24, 63};
        gKleeSpriteClips[3][1] = {117, 65, 22, 63};
        gKleeSpriteClips[3][2] = {293, 65, 26, 63};
        gKleeSpriteClips[3][3] = {422, 65, 26, 63};
        gKleeSpriteClips[3][4] = {549, 65, 27, 63};
        gKleeSpriteClips[3][5] = {676, 65, 30, 63};
        gKleeSpriteClips[3][6] = {807, 65, 25, 63};
        gKleeSpriteClipsSize[3] = 7;

        gKleeSpriteClips[4][0] = {49, 74, 28, 54};
        gKleeSpriteClips[4][1] = {179, 81, 22, 47};
        gKleeSpriteClips[4][2] = {309, 62, 26, 56};
        gKleeSpriteClips[4][3] = {441, 59, 23, 58};
        gKleeSpriteClips[4][4] = {561, 59, 34, 52};
        gKleeSpriteClips[4][5] = {691, 65, 32, 63};
        gKleeSpriteClips[4][6] = {817, 70, 31, 58};
        gKleeSpriteClips[4][7] = {954, 72, 26, 56};
        gKleeSpriteClipsSize[4] = 8;

        gKleeSpriteClips[5][0] = {49, 63, 25, 65};
        gKleeSpriteClips[5][1] = {178, 64, 22, 64};
        gKleeSpriteClips[5][2] = {305, 65, 34, 63};
        gKleeSpriteClips[5][3] = {431, 64, 36, 64};
        gKleeSpriteClips[5][4] = {560, 65, 36, 63};
        gKleeSpriteClips[5][5] = {689, 66, 34, 62};
        gKleeSpriteClips[5][6] = {818, 65, 33, 63};
        gKleeSpriteClipsSize[5] = 7;

        gKleeSpriteClips[6][0] = {13, 68, 35, 60};
        gKleeSpriteClips[6][1] = {126, 66, 49, 62};
        gKleeSpriteClips[6][2] = {267, 66, 33, 62};
        gKleeSpriteClips[6][3] = {399, 69, 24, 59};
        gKleeSpriteClips[6][4] = {524, 68, 86, 60};
        gKleeSpriteClips[6][5] = {652, 66, 113, 62};
        gKleeSpriteClips[6][6] = {778, 63, 115, 65};
        gKleeSpriteClips[6][7] = {910, 68, 114, 60};
        gKleeSpriteClips[6][8] = {1043, 69, 104, 60};
        gKleeSpriteClips[6][9] = {1164, 69, 113, 57};
        gKleeSpriteClips[6][10] = {1264, 69, 108, 59};
        gKleeSpriteClips[6][11] = {1424, 68, 101, 60};
        gKleeSpriteClipsSize[6] = 12;

        gKleeSpriteClips[7][0] = {42, 66, 33, 59};
        gKleeSpriteClips[7][1] = {167, 65, 34, 62};
        gKleeSpriteClips[7][2] = {290, 65, 47, 62};
        gKleeSpriteClips[7][3] = {424, 68, 41, 58};
        gKleeSpriteClips[7][4] = {561, 65, 31, 60};
        gKleeSpriteClips[7][5] = {678, 65, 38, 60};
        gKleeSpriteClips[7][6] = {803, 66, 48, 61};
        gKleeSpriteClips[7][7] = {936, 66, 30, 59};
        gKleeSpriteClipsSize[7] = 8;

        gKleeSpriteClips[8][0] = {48, 65, 22, 63};
        gKleeSpriteClips[8][1] = {177, 65, 21, 63};
        gKleeSpriteClips[8][2] = {305, 65, 20, 63};
        gKleeSpriteClips[8][3] = {434, 65, 20, 63};
        gKleeSpriteClips[8][4] = {561, 65, 20, 63};
        gKleeSpriteClips[8][5] = {689, 65, 23, 63};
        gKleeSpriteClips[8][6] = {818, 65, 20, 63};
        gKleeSpriteClipsSize[8] = 7;
    }
    string fname = fileKlee + "/" + fileLightBall + ".png";
    if(!gTLightBall.loadFromFile(fname, gRenderer)){
        cout << "Failed to load lightball!\n"; success = false;
    }
    else{
        gLightBallSpriteClips[0] = {27, 28, 10, 9};
        gLightBallSpriteClips[1] = {89, 19, 17, 27};
        gLightBallSpriteClips[2] = {153, 19, 18, 25};
        gLightBallSpriteClips[3] = {220, 21, 13, 13};
        gLightBallSpriteClips[4] = {284, 27, 6, 9};
        gLightBallSpriteClips[5] = {347, 29, 9, 9};
        gLightBallSpriteClips[6] = {405, 15, 21, 30};
        gLightBallSpriteClips[7] = {468, 18, 23, 27};
        gLightBallSpriteClips[8] = {532, 29, 15, 10};
    }
    gTitleFont = TTF_OpenFont("fonts/Helmet.ttf", 60);
    if(gTitleFont == NULL){
        cout << "Unable to load Helmet font! " << TTF_GetError() << '\n'; success = false;
    }
    gFont = TTF_OpenFont("fonts/Mgen+ 1pp regular.ttf", 24);
    if(gFont == NULL){
        cout << "Unable to load Mgen regular font! " << TTF_GetError() << '\n'; success = false;
    }
    if(!gTEnemy.loadFromFile("image/png/Object/Mushroom_1.png", gRenderer)){
        cout << "Failed to load mushroom!"; success = false;
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

bool setTiles(Tile* tiles[], string mapName, vector<Enemy>& mush1){
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
                    if((int)upDown.size() < 2){
                        movingGround tmp(lj * TILE_WIDTH, (j + 1) * TILE_WIDTH, i * TILE_HEIGHT, lj * TILE_WIDTH, (j + 1) * TILE_WIDTH, 64, 576, "ud");
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
    for(int i = 0; i < 3; i++){
        int id = rand() % (int)coor.size();
        int lx = coor[id].fi.fi; int y = coor[id].fi.se; int rx = coor[id].se.fi + 1;
        mush1.pb(Enemy(lx * TILE_WIDTH, y * TILE_HEIGHT - Enemy::ENEMY_HEIGHT + 10, rx * TILE_WIDTH - Enemy::ENEMY_WIDTH));
    }
    //cout << "----------------------------------------------------------------------\n";
    //cout << "Ground:\n";
    return success;
}

void close(Tile* tiles[]){
	//Free loaded image
	gTBackground.free();
	for(int i = 0; i < 11; i++){
        gTKlee[i].free();
	}
	gTTime.free();
	gTText.free();
	gTEnemy.free();
	gTEndLevelSign.free();
	gTMenuButtonsSpriteSheet[0].free();
	gTMenuButtonsSpriteSheet[1].free();
	for(int i = 0; i < TOTAL_TILE_TYPES; i++){
        gTypeTiles[i].free();
	}
	for(int i = 0; i < MAX_TOTAL_TILES; i++){
        if(tiles[i] != NULL){
            delete tiles[i];
            tiles[i] = NULL;
        }
	}
	mush1.clear();
	ground.clear();
	upDown.clear();
	menuStart.clear();
}
