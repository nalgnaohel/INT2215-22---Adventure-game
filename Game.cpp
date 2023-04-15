#include "Game.h"

Game::Game(string level, SDL_Rect& box)
{
    //ctor
    lv = level;
    if(mush1.size() > 0) mush1.clear();
    if(ground.size() > 0) ground.clear();
    for(int i = 0; i < MAX_TOTAL_TILES; i++){
        tiles[i] = NULL;
    }
    isRunning = false;
    klee = new Klee(box);
    camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}

Game::~Game()
{
    //dtor
    mush1.clear();
    ground.clear();
    for(int i = 0; i < TOTAL_TILES; i++){
        if(tiles[i] != NULL){
            delete tiles[i];
            tiles[i] = NULL;
        }
    }
    delete klee;
    klee = NULL;
}

bool Game::setTiles(){
    bool success = true;
    string fmap = "image/png/Map/map_" + lv + ".png";
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
    ground.pb(make_pair(make_pair(0, 9), 18));
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
                    //cout << "(" << i << ", " << j << ") - ";
                    li = i; lj = j;
                }
                if(intId == 7){
                    //cout << "(" << i << ", " << j << ")\n";
                    ground.pb(make_pair(make_pair(lj, j), i));
                    coor.pb(make_pair(make_pair(lj, li), make_pair(j, i)));
                    li = -1; lj = -1;
                }
                if(intId == 1){
                    li = i; lj = j;
                }
                if(intId == 3){
                    ground.pb(make_pair(make_pair(lj, j), i));
                    li = -1; lj = -1;
                }
                if(intId == 13){
                    li = i; lj = j;
                }
                if(intId == 15){
                    ground.pb(make_pair(make_pair(lj, j), i));
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
    TOTAL_TILES = tileId;
    srand(time(NULL));
    for(int i = 0; i < 3; i++){
        int id = rand() % (int)coor.size();
        int lx = coor[id].fi.fi; int y = coor[id].fi.se; int rx = coor[id].se.fi + 1;
        mush1.pb(Enemy(lx * TILE_WIDTH, y * TILE_HEIGHT - Enemy::ENEMY_HEIGHT + 10, rx * TILE_WIDTH - Enemy::ENEMY_WIDTH));
    }
    //cout << "----------------------------------------------------------------------\n";
    //cout << "Ground:\n";
    for(int i = 0; i < (int)ground.size(); i++){
        cout << ground[i].fi.fi * TILE_WIDTH << ' ' << ground[i].fi.se * TILE_WIDTH << ' ' << ground[i].se * TILE_HEIGHT << '\n';
    }
    return success;
}

int Game::run(SDL_Renderer* gRenderer){
    if(!setTiles(tiles, mush1)){
        cout << "Unable to load map!\n"; return;
    }
    isRunning = true;
    SDL_Event event;
    Timer fps_timer;
    int numOfCollision = 0;
    int scrollingOffset = 0;
    bool gameOver = 0;
    klee->TOTAL_TILES = TOTAL_TILES;
    while(isRunning){
        fps_timer.start();
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                isRunning = 0;
            }
            klee->handleKleeEvent(event);
        }
        klee->move(tiles, ground);
        //cout << "Jump: " << klee.isJump() << ' ' << klee.onGround(ground) << '\n';
        //cout << tiles[9 * (LEVEL_WIDTH/TILE_WIDTH) + 21]->getBox().x << ' ' << tiles[9 * (LEVEL_WIDTH/TILE_WIDTH) + 21]->getBox().y << ' ' << tiles[9 * (LEVEL_WIDTH/TILE_WIDTH) + 21]->getBox().x + tiles[9 * (LEVEL_WIDTH/TILE_WIDTH) + 21]->getBox().w << ' ' << tiles[9 * (LEVEL_WIDTH/TILE_WIDTH) + 21]->getBox().y + tiles[9 * (LEVEL_WIDTH/TILE_WIDTH) + 21]->getBox().h << " - ";
        //cout << klee.getKleeBox().x << ' ' << klee.getKleeBox().y << ' ' << klee.getKleeBox().x + klee.getKleeBox().w << ' ' << klee.getKleeBox().y +  klee.getKleeBox().h << '\n';
        klee->setCamera(camera);
        for(int i = 0; i < (int)mush1.size(); i++){
            mush1[i].move();
        }
        for(int i = 0; i < (int)mush1.size(); i++){
            SDL_Rect kBox = klee->getKleeBox(); SDL_Rect m1Box = mush1[i].getEnemyBox();
            if(checkCollision(kBox, m1Box)){
                numOfCollision++;
                if(numOfCollision == 10){
                    klee->updateKleeHealth(mush1[i].getEnemyDamage());
                    numOfCollision = 0;
                }
                //cout << numOfCollision << ' ' << klee.getKleeHealth() << '\n';
                if(klee->getKleeBox().x < m1Box.x){
                    mush1[i].dir = 0;
                }
                else{
                    mush1[i].dir = 1;
                }
                if(klee->getKleeLife() <= 0){
                    gameOver = 1;
                }
                if(gameOver) break;
            }
        }
        if(klee->getKleeBox().y > SCREEN_HEIGHT){
            gameOver = true;
        }

        //rendering
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
                tiles[i]->render(gRenderer, gTypeTiles, camera);
            }
        }
        SDL_Rect* currentClip = &gKleeSpriteClips[klee->spriteId][klee->kleeFrame / KLEE_FRAME_PER_SECOND];
        klee->render(gTKlee[klee->spriteId], gRenderer, currentClip, camera);
        for(int i = 0; i < (int)mush1.size(); i++){
            mush1[i].render(gTEnemy, gRenderer, camera);
        }
        SDL_RenderPresent(gRenderer);
        //update frame
        if(klee->spriteId != 4){
            klee->kleeFrame++;
            if(klee->kleeFrame /KLEE_FRAME_PER_SECOND >= gKleeSpriteClipsSize[klee->spriteId]){
                klee->kleeFrame = 0;
            }
            klee->updateKleeBox(gKleeSpriteClips[klee->spriteId][klee->.kleeFrame / KLEE_FRAME_PER_SECOND]);
        }
        int now_time = fps_timer.getCurTicks();
        int frameTime = 1000 / FRAME_PER_SECOND;
        //cout << now_time << ' ' << frameTime << '\n';
        if(now_time < frameTime){
            int dl = frameTime - now_time;
            SDL_Delay(dl);
        }
        if(gameOver){
            isRunning = false;
        }
        if(klee->getKleeBox().x + klee->getKleeBox().w == LEVEL_WIDTH){
            isRunning = false;
        }
    }
    if(gameOver){
        return 0;
    }
    return 1;
}
