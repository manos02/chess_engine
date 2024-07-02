#include "vars.h"

#ifndef INITATTACKS_H
#define INITATTACKS_H


void init_all();
int check_if_set(U64 board, int pos);
void set_bit(U64 *piece, int pos);
void remove_bit(U64 *piece, int pos);
U64 get_bishop_attacks(int pos, U64 blockers);
U64 get_queen_attacks(int pos, U64 blockers);
U64 get_rook_attacks(int pos, U64 blockers);
int is_square_attacked(int pos, int color);

#endif

