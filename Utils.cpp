#include "Utils.h"

bool checkCollision(SDL_Rect& a, SDL_Rect& b){
    int top_a, top_b;
    int bottom_a, bottom_b;
    int left_a, left_b;
    int right_a, right_b;
    top_a = a.y; top_b = b.y;
    bottom_a = a.y + a.h; bottom_b = b.y + b.h;
    left_a = a.x; left_b = b.x;
    right_a = a.x + a.w; right_b = b.x + b.w;
    if(bottom_a <= top_b){
        return 0;
    }
    if(top_a >= bottom_b){
        return 0;
    }
    if(left_a >= right_b){
        return 0;
    }
    if(right_a <= left_b){
        return 0;
    }
    return 1;
}

/*
how should data.txt looks like?
Klee: life - exp - lim_cd - last coor(x, y)
no of cleared Level
no of ghost0, ghost 1
coordinate of each ghost(0 ->1)(lx - y - rx) - health
*/
