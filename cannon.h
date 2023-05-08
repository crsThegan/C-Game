#ifndef CANNON_H
#define CANNON_H

#include "player.h"
#include "board.h"

#include <Windows.h>

struct Cannon {
    int x;
    int y;
    enum Direction dir;
};

extern const int FIRE_FREQ;
extern int cannonCount;
extern struct Cannon *cannons;

void cannon_create(int x, int y, enum Direction dir);
DWORD WINAPI cannons_shoot(LPVOID board);

#endif