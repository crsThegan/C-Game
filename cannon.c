#include "cannon.h"
#include "player.h"
#include "board.h"
#include "bullet.h"
#include "type_flags.h"

#include <stdlib.h>
#include <assert.h>
#include <Windows.h>

const int CANNON_RELOAD_TIME = 1000;
int cannonCount = 1;
struct Cannon *cannons;

static inline void cannon_shoot(struct Cannon *self, element (*board)[HEIGHT]);
static inline void cannon_init(struct Cannon *self, int x, int y, enum Direction dir);

void cannon_create(int x, int y, enum Direction dir) {
    struct Cannon *temp = realloc(cannons, ++cannonCount * sizeof(struct Cannon));
    assert(temp);
    cannons = temp;
    cannon_init(cannons + cannonCount - 1, x, y, dir);
}

DWORD WINAPI cannons_shoot(LPVOID board) {
    while (1) {
        for (int i = 1; i < cannonCount; i++) cannon_shoot(cannons + i, board);
        Sleep(CANNON_RELOAD_TIME);
    }
    return 0;
}

static inline void cannon_shoot(struct Cannon *self, element (*board)[HEIGHT]) {
    bullet_create(self, CANNON_FLAG);
}

static inline void cannon_init(struct Cannon *self, int x, int y, enum Direction dir) {
    self->x = x;
    self->y = y;
    self->dir = dir;
}