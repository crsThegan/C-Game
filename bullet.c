#include "bullet.h"
#include "player.h"
#include "board.h"
#include "cannon.h"
#include "type_flags.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

const int bulletRange = 5;

int bulCount = 1;
struct Bullet *bullets;

HANDLE bulletMutex; // needed to make sure that the 'bullets' arr is not being modified by
                    // multiple threads simultaneously

static inline void bullet_initFromPl(struct Bullet *self, struct Player *shooter);
static inline void bullet_initFromCannon(struct Bullet *self, struct Cannon *shooter);
static inline void bullet_destroy(struct Bullet *self);
static void bullet_fly(struct Bullet *self, element (*board)[HEIGHT]);

void bullet_create(void *shooter, tflag typeFlag) {
    WaitForSingleObject(bulletMutex, INFINITE);

    struct Bullet *temp = realloc(bullets, ++bulCount * sizeof(struct Bullet));
    assert(temp);
    bullets = temp;

    switch (typeFlag) {
    case PLAYER_FLAG:
        bullet_initFromPl(bullets + bulCount - 1, shooter);
        break;
    case CANNON_FLAG:
        bullet_initFromCannon(bullets + bulCount - 1, shooter);
        break;
    }

    ReleaseMutex(bulletMutex);
}

DWORD WINAPI bullets_fly(LPVOID board) {
    while (1) {
        for (int i = 1; i < bulCount; i++) { // the first bullet is needed everytime so the 'bullets' array doesn't
                                             // have the size of 0 for realloc() to work properly
            bullet_fly(bullets + i, board);
            --bullets[i].distLeft;
        }
    }
    return 0;
}

static inline void bullet_initFromPl(struct Bullet *self, struct Player *shooter) {
    self->x = shooter->x;
    self->y = shooter->y;
    self->dir = shooter->dir;

    self->distLeft = bulletRange;
}

static inline void bullet_initFromCannon(struct Bullet *self, struct Cannon *shooter) {
    self->x = shooter->x;
    self->y = shooter->y;
    self->dir = shooter->dir;

    self->distLeft = bulletRange;

    Sleep(FIRE_FREQ);
}

static inline void bullet_destroy(struct Bullet *self) {
    WaitForSingleObject(bulletMutex, INFINITE);

    memmove(self, self + 1, bulCount - (self - bullets) - 1); // ~ arr.pop(self); self - arr = index
    struct Bullet *temp = realloc(bullets, --bulCount * sizeof(struct Bullet));
    assert(temp);
    bullets = temp;

    ReleaseMutex(bulletMutex);
}

static void bullet_fly(struct Bullet *self, element (*board)[HEIGHT]) {
    switch (self->dir) {
    case RIGHT:
        if (board[self->x + 1][self->y] == BLANK_SPACE && self->distLeft > 0) self->x++;
        else bullet_destroy(self);
        break;
    case LEFT:
        if (board[self->x - 1][self->y] == BLANK_SPACE && self->distLeft > 0) self->x--;
        else bullet_destroy(self);
        break;
    case UP:
        if (board[self->x][self->y + 1] == BLANK_SPACE && self->distLeft > 0) self->y++;
        else bullet_destroy(self);
        break;
    case DOWN:
        if (board[self->x][self->y - 1] == BLANK_SPACE && self->distLeft > 0) self->y--;
        else bullet_destroy(self);
        break;
    }
}