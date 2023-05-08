#include "board.h"
#include "player.h"
#include "bullet.h"
#include "cannon.h"

#include <stdio.h>
#include <Windows.h>

element board[WIDTH][HEIGHT];

HANDLE bulletThread;
HANDLE cannonThread;
HANDLE playerThread;
struct ThreadArgs plThrArgs;
const int GAME_TICK = 40;

int main() {
    setup(board);

    while (1) {
        changeBoard(board);
        drawBoard(board);

        Sleep(GAME_TICK);
        system("cls");
    }

    terminate();
    system("pause");
    return 0;
}

void setup(element (*board)[HEIGHT]) {
    player_init(&player);

    bullets = calloc(1, sizeof(struct Bullet));

    cannons = calloc(1, sizeof(struct Cannon));

    bulletMutex = CreateMutex(NULL, FALSE, NULL);
    bulletThread = CreateThread(NULL, 0, bullets_fly, board, 0, NULL);
    cannonThread = CreateThread(NULL, 0, cannons_shoot, board, 0, NULL);
    threadArgs_init(&plThrArgs, &player, board);
    playerThread = CreateThread(NULL, 0, player_actionCheck, &plThrArgs, 0, NULL);

    for (int i = 0; i < 5; i++) cannon_create(WIDTH - 2, HEIGHT / 2 + i, LEFT);

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) board[x][y] = WALL;
            else board[x][y] = BLANK_SPACE;
        }
    }
}

void changeBoard(element (*board)[HEIGHT]) {
    for (int x = 1; x < WIDTH - 1; x++) {
        for (int y = 1; y < HEIGHT - 1; y++) {
            board[x][y] = BLANK_SPACE;
            if (x == player.x && y == player.y) {
                switch (player.dir) {
                case RIGHT:
                    board[x][y] = PLAYER_RIGHT;
                    break;
                case LEFT:
                    board[x][y] = PLAYER_LEFT;
                    break;
                case UP:
                    board[x][y] = PLAYER_UP;
                    break;
                case DOWN:
                    board[x][y] = PLAYER_DOWN;
                    break;
                }
            }
            else if (board[x][y] == BLANK_SPACE) {
                for (int i = 1; i < cannonCount; i++) {
                    if (x == cannons[i].x && y == cannons[i].y) {
                        if (cannons[i].dir == UP || cannons[i].dir == DOWN) board[x][y] = CANNON_VERTICAL;
                        else board[x][y] = CANNON_HORIZONTAL;
                    }
                }
            }
            else if (board[x][y] == BLANK_SPACE) {
                for (int i = 1; i < bulCount; i++) {
                    if (x == bullets[i].x && y == bullets[i].y) board[x][y] = BULLET;
                }
            }
        }
    }
}

void drawBoard(element (*board)[HEIGHT]) {
    char boardStr[HEIGHT][WIDTH + 1] = {0};
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WIDTH; x++) boardStr[y][x] = board[x][y];
        puts(boardStr[y]);
    }

    char tempbuf[32];
    sprintf(tempbuf, "%d", player.dir);
    puts(tempbuf);
}

void terminate() {
    free(bullets);
    free(cannons);

    CloseHandle(bulletThread);
    CloseHandle(cannonThread);
    CloseHandle(playerThread);
    CloseHandle(bulletMutex);
}