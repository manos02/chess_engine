#include "vars.h"
#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

// preserve board state
#define copy_board()                                                            \
  U64 bitboards_copy[12], occupancies_copy[3];                                \
  int to_move_copy, enpassant_copy, castle_copy;                              \
  memcpy(bitboards_copy, bitboards, 96);                                      \
  memcpy(occupancies_copy, occupancies, 24);                                  \
  to_move_copy = to_move, enpassant_copy = enpassant, castle_copy = castle;   \

// restore board state
#define take_back()                                                       \
  memcpy(bitboards, bitboards_copy, 96);                                \
  memcpy(occupancies, occupancies_copy, 24);                            \
  to_move = to_move_copy, enpassant = enpassant_copy, castle = castle_copy;   \



void move_generator(MoveList *move_list);
void set_bit(U64 *piece, int pos);
int make_move(U64 move, int move_flag);
void print_move_list(MoveList move_list);


#endif
