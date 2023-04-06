#ifndef UTILS_H
#define UTILS_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILE_TYPES = 19;
const int OBJECTS_ID[10] = {1, 2, 3, 4, 6, 7, 11, 13, 14, 15};
const int TOTAL_TILES = 2240;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int LEVEL_WIDTH = 3584;
const int LEVEL_HEIGHT = 640;
const int KLEE_WALKING_FRAME = 2;

enum mouseButtonsSprite{
    BUTTON_MOUSE_OUT = 0,
    BUTTON_MOUSE_OVER = 1,
    BUTTON_TOTAL = 2
};

bool checkCollision(SDL_Rect& a, SDL_Rect& b);

#endif
// UTILS_H
