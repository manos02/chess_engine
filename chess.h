#ifndef CHESS_H
#define CHESS_H


typedef struct Square Square;
// typedef unsigned long long  U64; 

typedef struct Piece {
    char type;
    int colour;
    Square *square;
} Piece;


typedef struct Square {
    int row;
    int column;
    Piece *piece;
} Square;

typedef struct Player {
    int colour;
    int name; 
} Player;

typedef struct Board {
    Square squares[8][8];
    int *wk;
    int *bk;
} Board;

typedef struct Move {
    Square *from_square;
    Square *to_square;
    Piece *captured_piece;
} Move;




#endif

