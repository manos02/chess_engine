#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "chess.h"


typedef unsigned long long  U64; 

enum {
  a8, b8, c8, d8, e8, f8, g8, h8,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a1, b1, c1, d1, e1, f1, g1, h1,
};

enum {
  white, black
};

Piece *new_piece(char type, int colour, Square *square) {
  Piece *piece = (Piece*) malloc(sizeof(Piece));
  piece->type = type;
  piece->colour = colour;
  piece->square = square;
  return piece;
}

void set_bit(U64 *piece, int pos) {
  (*piece) = (*piece) | (1ULL << pos);
}

void remove_bit(U64 *piece, int pos) {
  if ((*piece) & (1ULL << pos)) // chech if there is a piece to remove
  (*piece) = (*piece) ^ (1ULL << pos);
}

void print_board(U64 board) {
  
  U64 index = 1ULL;
  printf("  +-----------------+\n");
  for (int row=0;row<8;row++)  {
    printf("%d | ", 8 - row);
    for (int col=0;col<8;col++) {
      printf("%d ", (board & index) > 0 ? 1 : 0); 
      index = index << 1;   
    }
    printf("| %d\n", 8-row);
  }
  printf("  +-----------------+\n");
  printf("    a b c d e f g h\n");
}



void generate_pawn_attacks(int pos, int color) {

  U64 attack_board = 0ULL;
  U64 position_board = 0ULL;
  set_bit(&position_board, pos);

  print_board(position_board);

  if (!color) { // white
    attack_board = attack_board | (position_board >> 7);
    attack_board = attack_board | (position_board >> 9);

  } else { // black
    attack_board = attack_board | (position_board << 7);
    attack_board = attack_board | (position_board << 9);
  }
  print_board(attack_board);


}











int main(int argc, char *argv[]) {



  U64 white_pawns = 0ULL;
  // set_bit(&white_pawns, a3);

  generate_pawn_attacks(d3, black);
  // print_board(white_pawns);



  return 0;
}