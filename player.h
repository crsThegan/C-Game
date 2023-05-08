#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"

#include <Windows.h>

// controls
#define RIGHT_KEY 'd'
#define LEFT_KEY 'a'
#define DOWN_KEY 's'
#define UP_KEY 'w'
#define SHOOT_KEY 'r'

enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

struct Player {
    int x;
    int y;
    enum Direction dir;

    int ammo;
    int health;
};

struct ThreadArgs {
    struct Player *player;
    element (*board)[HEIGHT];
};

extern struct Player player;

void player_init(struct Player *self);
void player_hit(struct Player *self, int damage);
void player_shoot(struct Player *self);

DWORD WINAPI player_actionCheck(LPVOID argsVoid);

#endif