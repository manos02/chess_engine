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

const U64 not_a_col = 18374403900871474942ULL;
const U64 not_h_col = 9187201950435737471ULL;
const U64 not_ab_col = 18229723555195321596ULL;
const U64 not_gh_col = 4557430888798830399ULL;
const U64 not_8_row = 18446744073709551360ULL;
const U64 not_87_row = 18446744073709486080ULL;
const U64 not_1_row = 72057594037927935ULL;
const U64 not_12_row = 281474976710655ULL;

U64 pawn_attacks[2][64];
U64 knight_attacks[64];


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




U64 generate_pawn_attacks(int pos, int color) {

  U64 attack_board = 0ULL;
  U64 position_board = 0ULL;
  set_bit(&position_board, pos);
  
  if (!color) { // white
    if ((position_board & not_a_col)) attack_board |= (position_board >> 9); // pawn not in a row
    if ((position_board & not_h_col)) attack_board |= (position_board >> 7); // pawn not in h row
  } else { // black
    if ((position_board & not_h_col)) attack_board |= (position_board << 9); 
    if ((position_board & not_a_col)) attack_board |= (position_board << 7); 
  }
  return attack_board;
}

void init_pawn_attacks() {
  for (int i=0; i<64; i++) {
    pawn_attacks[white][i] = generate_pawn_attacks(i, white);
    pawn_attacks[black][i] = generate_pawn_attacks(i, black);
  }
}


U64 generate_knight_attacks(int pos) {

  U64 attack_board = 0ULL;
  U64 position_board = 0ULL;
  set_bit(&position_board, pos);
  print_board(position_board);
  
 
  if ((position_board & not_gh_col)) attack_board |= (position_board >> 6); // 1 up 2 right
  if ((position_board & not_ab_col)) attack_board |= (position_board >> 10); // 1 up 2 left
  if ((position_board & not_a_col)) attack_board |= (position_board >> 17); // 2 up 1 left
  if ((position_board & not_h_col)) attack_board |= (position_board >> 15); // 2 up 1 right

  if ((position_board & not_ab_col)) attack_board |= (position_board << 6); // 1 down 2 left
  if ((position_board & not_gh_col)) attack_board |= (position_board << 10); // 1 down 2 right
  if ((position_board & not_h_col)) attack_board |= (position_board << 17); // 2 down 1 right
  if ((position_board & not_a_col)) attack_board |= (position_board << 15); // 2 down 1 left
    
  return attack_board;
}

void init_knight_attacks() {
  for (int i=0; i<64; i++) {
    knight_attacks[i] = generate_knight_attacks(i);
  }
}



int main(int argc, char *argv[]) {



  U64 white_pawns = 0ULL;
  // set_bit(&white_pawns, a3);

  // generate_pawn_attacks(h8, white);
  generate_knight_attacks(a1);

  init_pawn_attacks();
  init_knight_attacks();

  // for (int i=0; i<64; i++) {
  //   print_board(knight_attacks[i]);
  //   printf("%d\n", i);  
  // }

  // print_board(white_pawns);
  // int pos = 0;
  // for (int row=0;row<8;row++)  {
  //   for (int col=0;col<8;col++) {
  //     if (row > 1) {
  //       set_bit(&white_pawns, pos);
  //     }
  //     pos++;
  //   }
  // } 
  // print_board(white_pawns);
  // printf("%llu\n", white_pawns);



  return 0;
}