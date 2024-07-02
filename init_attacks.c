#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "vars.h"


/* Example, Rook on e4:
 *  
 *    The blocker mask        A blocker board         The move board
 *    0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0 
 *    0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0         0 0 0 0 0 0 0 0 
 *    0 0 0 0 1 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0 
 *    0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0 
 *    0 1 1 1 0 1 1 0         0 1 1 0 0 0 0 0         0 0 1 1 0 1 1 1 
 *    0 0 0 0 1 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 1 0 0 0 
 *    0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0 
 *    0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0 
 */


void set_bit(U64 *piece, int pos) {
  (*piece) = (*piece) | (1ULL << pos);
}

void remove_bit(U64 *piece, int pos) {
  if ((*piece) & (1ULL << pos)) // check if there is a piece to remove
  (*piece) = (*piece) ^ (1ULL << pos);
}

int check_if_set(U64 board, int pos) {
  if (board & (1ULL << pos)) {
    return 1;
  }
  return 0;
}




U64 singlePushTargets(U64 wpawns, U64 empty, int color) {
  U64 board_pawn_push = color == white ? (wpawns >> 8) & empty : (wpawns << 8) & empty;
  return board_pawn_push;
}


U64 wDblPushTargets(U64 wpawns, U64 empty, int color) {
  U64 singlePushs = singlePushTargets(wpawns, empty, color);
  U64 board_pawn_push = color == white ? ((singlePushs >> 8) & empty & row_4) : (singlePushs << 8) & empty & row_5;
  return board_pawn_push;
}



U64 mask_bishop(int col, int row) {
  U64 bishop_board = 0ULL;

  int temp_col = col + 1;
  int temp_row = row + 1;
  while (temp_row < 7 && temp_col < 7) {
    set_bit((&bishop_board), temp_row * 8 + temp_col);
    temp_col++;
    temp_row++;
  }

  temp_col = col - 1;
  temp_row = row - 1;
  while (temp_row > 0 && temp_col > 0) {
    set_bit((&bishop_board), temp_row * 8 + temp_col);
    temp_col--;
    temp_row--;
  }

  temp_col = col - 1;
  temp_row = row + 1;
  while (temp_row < 7 && temp_col > 0) {
    set_bit((&bishop_board), temp_row * 8 + temp_col);
    temp_col--;
    temp_row++;
  }

  temp_col = col + 1;
  temp_row = row - 1;
  while (temp_row > 0 && temp_col < 7) {
    set_bit((&bishop_board), temp_row * 8 + temp_col);
    temp_col++;
    temp_row--;
  }
  return bishop_board;
}

U64 mask_rook(int col, int row) {
  U64 rook_board = 0ULL;


  for (int temp_col = col + 1; temp_col < 7; temp_col++)  set_bit((&rook_board), row * 8 + temp_col);
  for (int temp_col = col - 1; temp_col > 0; temp_col--)  set_bit((&rook_board), row * 8 + temp_col);
  for (int temp_row = row + 1; temp_row < 7; temp_row++)  set_bit((&rook_board),  temp_row * 8 + col);
  for (int temp_row = row - 1; temp_row > 0; temp_row--)  set_bit((&rook_board),  temp_row * 8 + col);

  return rook_board;
}



U64 generate_blocker_boards(int index, U64 blocker_mask) {

  U64 blocker_board = blocker_mask;
  int bitindex = 0;

  for (int i=0; i<64; i++) {
    if (blocker_mask & (1ULL<<i)) { // check if the bit is set
      if (!(index & (1<<bitindex)) ) { // Clear the i'th bit in the blockerboard if it's clear in the index at bitindex
        blocker_board &= ~(1ULL<<i); //Clear the bit.
      }
      bitindex++;
    }
  }
  return blocker_board;
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



U64 generate_knight_attacks(int pos) {

  U64 attack_board = 0ULL;
  U64 position_board = 0ULL;
  set_bit(&position_board, pos);
   
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

U64 generate_king_attacks(int pos) {

  U64 attack_board = 0ULL;
  U64 position_board = 0ULL;
  set_bit(&position_board, pos);
   
  attack_board |= (position_board >> 8); // 1 up
  attack_board |= (position_board << 8); // 1 down
  if ((position_board & not_a_col)) attack_board |= ((position_board >> 9) | (position_board >> 1) | (position_board << 7)); // 1 up 1 left, 1 left, 1 down 1 left
  if ((position_board & not_h_col)) attack_board |= ((position_board >> 7) | (position_board << 1) | (position_board << 9)); // 1 up 1 right, 1 right, 1 down 1 right
    
  return attack_board;
}



U64 get_bishop_attacks(int pos, U64 blockers) {

  blockers &= BISHOP_MASKS[pos];
  U64 key = (blockers * BISHOP_MAGICS[pos]) >> (64 - BISHOP_INDEX_BITS[pos]);
  return bishop_attacks[pos][key];
}


U64 get_rook_attacks(int pos, U64 blockers) {

  blockers &= ROOK_MASKS[pos];
  U64 key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
  return rook_attacks[pos][key];
}


U64 get_queen_attacks(int pos, U64 blockers) {

  U64 queen_attacks = 0ULL;

  U64 bishop_blockers = blockers;
  U64 rook_blockers = blockers;

  bishop_blockers &= BISHOP_MASKS[pos];
  
  U64 key = (bishop_blockers * BISHOP_MAGICS[pos]) >> (64 - BISHOP_INDEX_BITS[pos]);
  queen_attacks = bishop_attacks[pos][key];
  
  rook_blockers &= ROOK_MASKS[pos];
  key = (rook_blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
  queen_attacks |= rook_attacks[pos][key];
  
  return queen_attacks;
}



U64 generate_rook_legal_bitboard(U64 blockers, int col, int row) {  

  U64 attack_board = 0ULL;

  for (int temp_col = col + 1; temp_col <= 7; temp_col++)  {
    int current = row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers,  current)) {
      break;
    }
  }

  for (int temp_col = col - 1; temp_col >= 0; temp_col--)  {
    int current = row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers,  current)) {
      break;
    }
  }

  for (int temp_row = row + 1; temp_row <= 7; temp_row++)  {
    int current = temp_row * 8 + col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers,  current)) {
      break;
    }
  }

  for (int temp_row = row - 1; temp_row >= 0; temp_row--)  {
    int current = temp_row * 8 + col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers,  current)) {  
      break;
    }
  }

  return attack_board;
}



U64 generate_bishop_legal_bitboard(U64 blockers, int col, int row) {  

  U64 attack_board = 0ULL;

  int temp_col, temp_row, current;

  temp_col = col + 1;
  temp_row = row + 1;
  while (temp_row <= 7 && temp_col <= 7) {
    current = temp_row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers, current)) {
      break;
    }
    temp_col++;
    temp_row++;
  }

  temp_col = col - 1;
  temp_row = row - 1;
  while (temp_row >= 0 && temp_col >= 0) {
    current = temp_row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers, current)) {
      break;
    }
    temp_col--;
    temp_row--;
  }


  temp_col = col + 1;
  temp_row = row - 1;
  while (temp_row >= 0 && temp_col <= 7) {
    current = temp_row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers, current)) {
      break;
    }
    temp_col++;
    temp_row--;
  }

  temp_col = col - 1;
  temp_row = row + 1;
  while (temp_row <= 7 && temp_col >= 0) {
    current = temp_row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers, current)) {
      break;
    }
    temp_col--;
    temp_row++;
  }

  return attack_board;
}



void init_leaping_pieces_attacks() {
  for (int i=0; i<64; i++) {
    pawn_attacks[white][i] = generate_pawn_attacks(i, white);
    pawn_attacks[black][i] = generate_pawn_attacks(i, black);
    knight_attacks[i] = generate_knight_attacks(i);
    king_attacks[i] = generate_king_attacks(i);
  }
}



// generate blocker masks for bishop, rooks and queens
void init_sliders_attacks(int is_bishop) {

  for (int row=0; row<8; row++) {
    for (int col=0; col<8; col++) {
      int pos = row * 8 + col;

      BISHOP_MASKS[pos] = mask_bishop(col, row);
      ROOK_MASKS[pos] = mask_rook(col, row);  
      QUEEN_MASKS[pos] = ROOK_MASKS[pos] | BISHOP_MASKS[pos];

      U64 attack_mask = is_bishop == 1 ? BISHOP_MASKS[pos] : ROOK_MASKS[pos];
      int relevant_bits = is_bishop == 1 ? BISHOP_INDEX_BITS[pos] : ROOK_INDEX_BITS[pos];
    
      
      for (int i=0; i < (1<<relevant_bits); i++) { // 2^relevant_bits blocker boards
        
        U64 blockers = generate_blocker_boards(i, attack_mask);

        if (is_bishop) {
          U64 key = (blockers * BISHOP_MAGICS[pos]) >> (64 - BISHOP_INDEX_BITS[pos]);
          bishop_attacks[pos][key] = generate_bishop_legal_bitboard(blockers, col, row);
      
        } else {
          U64 key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
          rook_attacks[pos][key] = generate_rook_legal_bitboard(blockers, col, row);
        }
      }
    }
  }
}


int is_square_attacked(int pos, int color) {
  
  if ((color == white && (pawn_attacks[black][pos] & bitboards[P]))) return 1; // attacked by white pawns
  if ((color == black && (pawn_attacks[white][pos] & bitboards[p]))) return 1;// attacked by black pawns

  if (knight_attacks[pos] & ((color == white) ? bitboards[N] : bitboards[n])) return 1; // attacked by knights
  if (king_attacks[pos] & ((color == white) ? bitboards[K] : bitboards[k])) return 1; // attacked by kings

  if (get_bishop_attacks(pos, occupancies[both]) & ((color == white) ? bitboards[B] : bitboards[b])) return 1; // attacked by bishops
  if (get_rook_attacks(pos, occupancies[both]) & ((color == white) ? bitboards[R] : bitboards[r])) return 1; // attacked by rooks
  if (get_queen_attacks(pos, occupancies[both]) & ((color == white) ? bitboards[Q] : bitboards[q])) return 1; // attacked by rooks

  return 0;
}





void init_all() {
  init_sliders_attacks(1); // bishop
  init_sliders_attacks(0); // rook
  init_leaping_pieces_attacks();
}


