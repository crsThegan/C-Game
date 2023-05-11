#include "player.h"
#include "board.h"
#include "bullet.h"
#include "type_flags.h"

#include <conio.h>
#include <Windows.h>

const int INITIAL_X = 1, INITIAL_Y = 1;
const enum Direction INITIAL_DIR = RIGHT;

const int INITIAL_AMMO;
const int INITIAL_HEALTH;

static void player_moveOnBoard(struct Player *self, element (*board)[HEIGHT]);

void threadArgs_init(struct ThreadArgs *self, struct Player *player, element (*board)[HEIGHT]) {
    self->player = player;
    self->board = board;
}

void player_init(struct Player *self) {
    self->x = INITIAL_X;
    self->y = INITIAL_Y;
    self->dir = INITIAL_DIR;
    self->ammo = INITIAL_AMMO;
    self->health = INITIAL_HEALTH;
}

void player_shoot(struct Player *self) {
    bullet_create(self, PLAYER_FLAG);
}

DWORD WINAPI player_actionCheck(LPVOID argsVoid) {
    struct ThreadArgs *args = argsVoid;
    while (1) {
        if (kbhit()) {
            switch (getch()) {
            case RIGHT_KEY:
                args->player->dir = RIGHT;
                player_moveOnBoard(args->player, args->board);
                break;
            case LEFT_KEY:
                args->player->dir = LEFT;
                player_moveOnBoard(args->player, args->board);
                break;
            case UP_KEY:
                args->player->dir = UP;
                player_moveOnBoard(args->player, args->board);
                break;
            case DOWN_KEY:
                args->player->dir = DOWN;
                player_moveOnBoard(args->player, args->board);
                break;
            case SHOOT_KEY:
                player_shoot(args->player);
                break;
            }
            
            Sleep(GAME_TICK * 2);
        }
    }
}

static void player_moveOnBoard(struct Player *self, element (*board)[HEIGHT]) {
    switch (self->dir) {
    case RIGHT:
        if (board[self->x + 1][self->y] == BLANK_SPACE) self->x++;
        break;
    case LEFT:
        if (board[self->x - 1][self->y] == BLANK_SPACE) self->x--;
        break;
    case UP:
        if (board[self->x][self->y + 1] == BLANK_SPACE) self->y++;
        break;
    case DOWN:
        if (board[self->x][self->y - 1] == BLANK_SPACE) self->y--;
        break;
    }
}

struct Player player;