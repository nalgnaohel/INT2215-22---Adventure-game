#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
using namespace std;

#define pb push_back
#define JUMP 1
#define FALL 2
#define fi first
#define se second

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILE_TYPES = 19;
const int OBJECTS_ID[10] = {1, 2, 3, 7, 11, 13, 14, 15};
const int GROUND_ID[9] = {1, 2, 3, 7, 11, 13, 14, 15};
const int MAX_TOTAL_TILES = 4480;
const int FRAME_PER_SECOND = 50;
const int MAX_FALL_SPEED = 10;
const int GRAVITY_SPEED = 1;
const int KLEE_FRAME_PER_SECOND = 20;

const int SCREEN_WIDTH = 960; //1000
const int SCREEN_HEIGHT = 640; //544
const int SCREEN_BPP = 32;

const int LEVEL_WIDTH = 3584;
const int LEVEL_HEIGHT = 640;
const int KLEE_WALKING_FRAME = 7;

const int TOTAL_MENU_BUTTONS = 2;
const int MENU_BUTTON_WIDTH = 418;
const int MENU_BUTTON_HEIGHT = 102;

enum ButtonsSprite{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_UP = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 3
};

//map<pair<int, int>, bool> ckTile;
bool checkCollision(SDL_Rect& a, SDL_Rect& b);

#endif
// UTILS_H
