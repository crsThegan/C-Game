#ifndef BULLET_H
#define BULLET_H

#include "player.h"
#include "type_flags.h"

#include <Windows.h>

struct Bullet {
    int x;
    int y;
    enum Direction dir;

    int distLeft;
};

extern int bulCount;
extern struct Bullet *bullets;
extern HANDLE bulletMutex;

void bullet_create(void *shooter, tflag typeFlag);
DWORD WINAPI bullets_fly(LPVOID board);

#endif