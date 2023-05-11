#ifndef BOARD_H
#define BOARD_H

#define WIDTH 50
#define HEIGHT 20

#define BLANK_SPACE ' '

#define PLAYER_RIGHT '>'
#define PLAYER_LEFT '<'
#define PLAYER_UP '^'
#define PLAYER_DOWN 'v'

#define WALL '#'
#define BULLET 'o'
#define CANNON_HORIZONTAL '-'
#define CANNON_VERTICAL '|'

typedef char element;

extern element board[WIDTH][HEIGHT];
extern const int GAME_TICK;

// extern char tempbuf[];

void setup(element (*board)[HEIGHT]);
void changeBoard(element (*board)[HEIGHT]);
void drawBoard(element (*board)[HEIGHT]);
void terminate();

#endif