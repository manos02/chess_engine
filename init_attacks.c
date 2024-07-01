#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "init_attacks.h"

/*
                            WHITE PIECES


        Pawns                  Knights              Bishops
        
  8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0
  7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  1 1 1 1 1 1 1 1    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  0 0 0 0 0 0 0 0    1  0 1 0 0 0 0 1 0    1  0 0 1 0 0 1 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h


         Rooks                 Queens                 King

  8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0
  7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  1 0 0 0 0 0 0 1    1  0 0 0 1 0 0 0 0    1  0 0 0 0 1 0 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h


                            BLACK PIECES


        Pawns                  Knights              Bishops
        
  8  0 0 0 0 0 0 0 0    8  0 1 0 0 0 0 1 0    8  0 0 1 0 0 1 0 0
  7  1 1 1 1 1 1 1 1    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h


         Rooks                 Queens                 King

  8  1 0 0 0 0 0 0 1    8  0 0 0 1 0 0 0 0    8  0 0 0 0 1 0 0 0
  7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h



                             OCCUPANCIES


     White occupancy       Black occupancy       All occupancies

  8  0 0 0 0 0 0 0 0    8  1 1 1 1 1 1 1 1    8  1 1 1 1 1 1 1 1
  7  0 0 0 0 0 0 0 0    7  1 1 1 1 1 1 1 1    7  1 1 1 1 1 1 1 1
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  1 1 1 1 1 1 1 1    2  0 0 0 0 0 0 0 0    2  1 1 1 1 1 1 1 1
  1  1 1 1 1 1 1 1 1    1  0 0 0 0 0 0 0 0    1  1 1 1 1 1 1 1 1

*/




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


#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "



// piece bitboards
U64 bitboards[12];

// occupancy bitboards
U64 occupancies[3];

// blocker masks 
U64 BISHOP_MASKS[64];
U64 ROOK_MASKS[64];
U64 QUEEN_MASKS[64];

// attack tables
U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];
U64 bishop_attacks[64][1024];
U64 rook_attacks[64][4096];

// side to move
int to_move;

// enpassant square
int enpassant; 

// castling rights
int castle;

int halfmove_clock = 0; // the number of halfmoves sice the last capture or pawn advance used for the fifty rule

int fullmove_num = 0; // the number of full moves 

enum { wk = 1, wq = 2, bk = 4, bq = 8 }; // castling


// convert ASCII character pieces to encoded constants
int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k
};

// promoted pieces
char promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n'
};

// unicode pieces
char *unicode_pieces[12] = {"♟︎", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔",};

const int lsb_64_table[64] =
{
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};

/**
 * bitScanForward
 * @author Matt Taylor (2003)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int bitScanForward(U64 bb) {
   unsigned int folded;
   assert (bb != 0);
   bb ^= bb - 1;
   folded = (int) bb ^ (bb >> 32);
   return lsb_64_table[folded * 0x78291ACF >> 26];
}


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



void print_bitboard(U64 board) {
  
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


void fen_parser(char *fen) {

  // reset game state variables
  to_move = 0; 
  enpassant = -1;
  castle = 0;

  
  memset(bitboards, 0ULL, sizeof(bitboards)); // initialise bitboards
  memset(occupancies, 0ULL, sizeof(occupancies)); // initialise occupancies

  int pos = 0;

  int i=0;
  
  for (i = 0; i < strlen(fen); i++) {

    if (fen[i] == 32) {
      break; // space detected
    }
        
    if ((fen[i] >= 'a' && fen[i] <= 'z') || (fen[i] >= 'A' && fen[i] <= 'Z')) { // check if it is a piece 
        
      // init piece type
      int piece = char_pieces[fen[i]];
      
      // set piece on corresponding bitboard
      set_bit(&bitboards[piece], pos);
      
      pos++;
    } else if (fen[i] >= '0' && fen[i] <= '9') {
                    
      int offset = fen[i] - '0'; // convert char to int
      pos += offset;
    }       
  }

  i++;
  to_move = fen[i] == 'w' ? white : black; // side to move

  i+=2;
  
  while (fen[i] != 32) {
    if (fen[i] == 'K') {
      castle |= wk;
    } else if  (fen[i] == 'Q') {
      castle |= wq;
    } else if (fen[i] == 'k') {
      castle |= bk;
    } else if (fen[i] == 'q') {
      castle |= bq;
    }
    i++;
  }
  i++;

  
  if (fen[i] == '-') {
    enpassant = -1;
    i+=2;
  } else {
    enpassant = (8-(fen[i+1] - '0'))*8 + (fen[i] - 97);
    printf("%d\n", enpassant);
    i+=3;
  }


  halfmove_clock = fen[i];
  fullmove_num = fen[i+2];

  occupancies[black] |= bitboards[p];
  occupancies[black] |= bitboards[k];
  occupancies[black] |= bitboards[n];
  occupancies[black] |= bitboards[q];
  occupancies[black] |= bitboards[b];
  occupancies[black] |= bitboards[r];

  occupancies[white] |= bitboards[P];
  occupancies[white] |= bitboards[K];
  occupancies[white] |= bitboards[N];
  occupancies[white] |= bitboards[Q];
  occupancies[white] |= bitboards[B];
  occupancies[white] |= bitboards[R];

  occupancies[both] |= occupancies[white]; 
  occupancies[both] |= occupancies[black]; 
}

void print_board() {
    printf("  +-----------------+\n");
    for (int row=0;row<8;row++)  {
      printf("%d | ", 8 - row);
      for (int col=0;col<8;col++) {
        int pos = row*8+col;
        int piece_at_pos = -1;
        for (int piece=P; piece <= k; piece++) {
          if (check_if_set(bitboards[piece], pos)) {
            piece_at_pos = piece;
            break;
          }
        }
  
        printf("%s ", (piece_at_pos == -1) ? "." : unicode_pieces[piece_at_pos]);       
      }

      printf("| %d\n", 8-row);
    }
    printf("  +-----------------+\n");
    printf("    a b c d e f g h\n");

    // print side to move
    printf("     Side:     %s\n", !to_move ? "white" : "black");
    
    // print enpassant square
    printf("     Enpassant:   %s\n", (enpassant != -1) ? square_to_coordinates[enpassant] : "no");
    
    // print castling rights
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) ? 'K' : '-',
                                           (castle & wq) ? 'Q' : '-',
                                           (castle & bk) ? 'k' : '-',
                                           (castle & bq) ? 'q' : '-');
    
}

/*
          binary move bits                               hexidecimal constants
    
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/


U64 encode_move(int source, int target, int piece, int promoted, int capture, int doubl, int en, int castling) {

  U64 move = 0ULL;

  move |= source;
  move |= (target << 6);
  move |= (piece << 12);
  move |= (promoted << 16);
  

  if (capture) {
    move |= (1 << 20);
  } else if (doubl) {
    move |= (1 << 21);
  } else if (en) {
    move |= (1 << 22);
  } else if (castling) {
    move |= (1 << 23);
  }

  return move;
}

int decode_source_square(U64 move) {
  int source = move & 0x3f;
  return source;
}

int decode_target_square(U64 move) {
  int target = (move & 0xfc0) >> 6;
  return target;
}

int decode_piece(U64 move) {
  int piece = (move & 0xf000) >> 12;
  return piece;
}

int decode_promoted_piece(U64 move) {
  int promoted_piece = (move & 0xf0000) >> 16;
  return promoted_piece;
}

int decode_capture_flag(U64 move) {
  int capture_flag = (move & 0x100000) >> 20;
  return capture_flag;
}

int decode_dp_flag(U64 move) {
  int dp_flag = (move & 0x200000) >> 21;
  return dp_flag;
}

int decode_enpassant_flag(U64 move) {
  int enpassant_flag = (move & 0x400000) >> 22;
  return enpassant_flag;
}

int decode_castling_flag(U64 move) {
  int castling_flag = (move & 0x800000) >> 23;
  return castling_flag;
}


void add_move(MoveList *move_list, U64 move) {
  move_list->moves[move_list->move_count] = move;
  move_list->move_count++;
}

void print_move_list(MoveList move_list) {

  if (move_list.move_count==0) {
    printf("\n     No move in the move list!\n");
    return;
  }

  printf("\n     move    piece     capture   double    enpass    castling\n\n");
  
  for (int i=0; i < move_list.move_count; i++) {

    U64 move = move_list.moves[i]; // get move
    

    printf("     %s%s%c      %s         %d         %d         %d         %d\n", square_to_coordinates[decode_source_square(move)],
                                                                                  square_to_coordinates[decode_target_square(move)],
                                                                                  decode_promoted_piece(move) ? promoted_pieces[decode_promoted_piece(move)] : ' ',
                                                                                  unicode_pieces[decode_piece(move)],
                                                                                  decode_capture_flag(move),
                                                                                  decode_dp_flag(move),
                                                                                  decode_enpassant_flag(move),
                                                                                  decode_castling_flag(move));
  }

  printf("\n\n     Total number of moves: %d\n\n", move_list.move_count);
}


void generate_pawn_moves(U64 bitboard, int color, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks;
  int piece = (color==white? P : p);

  while (bitboard != 0ULL) {
    
    // init source square
    source_square = bitScanForward(bitboard);
                  
    // init target square
    target_square = color == white ? (source_square - 8) : (source_square + 8); // - 8 for white + 8 for black
    int upper_boundary = color == white ? target_square >= a8 : target_square <= h1; // upper boundary 0 violates rule

    if (upper_boundary && !check_if_set(occupancies[both], target_square)) { // check boundary and no other piece at target
      if (source_square >= (color == white ? a7 : a2) && source_square <= (color == white ? h7 : h2)) { // pawn upgrade
        add_move(move_list, encode_move(source_square, target_square, piece, (color==white? Q : q), 0, 0, 0, 0));
        add_move(move_list, encode_move(source_square, target_square, piece, (color==white? N : n), 0, 0, 0, 0));
        add_move(move_list, encode_move(source_square, target_square, piece, (color==white? B : b), 0, 0, 0, 0));
        add_move(move_list, encode_move(source_square, target_square, piece, (color==white? R : r), 0, 0, 0, 0));
      } else { // not a promotion
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

        // two squares ahead pawn move
        int double_pawn_target = target_square + (color == white ? -8 : 8);
    
        if ((source_square >= (color == white ? a2 : a7) && source_square <= (color == white ? h2 : h7)) && !check_if_set(occupancies[both], double_pawn_target)) 
          add_move(move_list, encode_move(source_square, double_pawn_target, piece, 0, 0, 1, 0, 0));
      }     
    }

    // init pawn attacks bitboard
  attacks = pawn_attacks[color][source_square] & (color == white ? occupancies[black] : occupancies[white]);

  while (attacks) {
    target_square = bitScanForward(attacks);

    if (source_square >= (color == white ? h7 : a2) && source_square <= (color == white ? h7 : h2)) { // pawn upgrade
      add_move(move_list, encode_move(source_square, target_square, piece, (color==white? Q : q), 1, 0, 0, 0));
      add_move(move_list, encode_move(source_square, target_square, piece, (color==white? N : n), 1, 0, 0, 0));
      add_move(move_list, encode_move(source_square, target_square, piece, (color==white? B : b), 1, 0, 0, 0));
      add_move(move_list, encode_move(source_square, target_square, piece, (color==white? R : r), 1, 0, 0, 0));

    } else {
      // one square ahead pawn move
      add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

    }

    remove_bit(&attacks, target_square);
  }

  if (enpassant != -1) {
    U64 enpassant_attacks = pawn_attacks[color][source_square] & (1ULL << enpassant); 

    // make sure enpassant capture available
    if (enpassant_attacks) {
        // init enpassant capture target square
        int target_enpassant = bitScanForward(enpassant_attacks);
        add_move(move_list, encode_move(source_square, target_square, piece, (color==white? R : r), 0, 0, 1, 0));

    }
  }

  remove_bit(&bitboard, source_square);
  }
  printf("\n");

}

void genereate_bishop_moves(U64 bitboard, int color, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (color==white ? B : b);


  while (bitboard != 0ULL) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite colour squares
    target_squares = get_bishop_attacks(source_square, occupancies[both]) & ((color == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (color == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {

      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }

      remove_bit(&target_squares, target_square);
    }
    remove_bit(&bitboard, source_square);
  }
}

void generate_rook_moves(U64 bitboard, int color, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (color==white? R : r);


  while (bitboard != 0ULL) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite color squares
    target_squares = get_rook_attacks(source_square, occupancies[both]) & ((color == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (color == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) {
        add_move(move_list, encode_move(target_square, piece, 0, 1, 0, 0, 0, 0));
      } else {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }
      remove_bit(&target_squares, target_square);
    }

    remove_bit(&bitboard, source_square);
  }
}

void generate_knight_moves(U64 bitboard, int color, MoveList *move_list) {
  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (color==white? N : n);


  while (bitboard != 0ULL) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite color squares
    target_squares = knight_attacks[source_square] & ((color == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (color == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }
      remove_bit(&target_squares, target_square);
    }
    remove_bit(&bitboard, source_square);
  }
}

void generate_queen_moves(U64 bitboard, int color, MoveList *move_list) {
  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (color==white ? Q : q);

  while (bitboard != 0ULL) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite color squares
    target_squares = get_queen_attacks(source_square, occupancies[both]) & ((color == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (color == white ? occupancies[black] : occupancies[white]);
    

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }
      remove_bit(&target_squares, target_square);
    }
    remove_bit(&bitboard, source_square);
  }
}

void generate_king_moves(U64 bitboard, int color, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks, target_squares;

  if ((castle & wk) && color == white) { // king side castling
    if (!check_if_set(occupancies[both], f1) && !check_if_set(occupancies[both], g1)) { // squares between rook and king are not occupied
      // make sure king and the f1 squares are not under attacks
      if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
        add_move(move_list, encode_move(e1, g1, K, 0, 0, 0, 0, 1));
    }
  }

  if ((castle & wq) && color == white) { // queen side castling
    if (!check_if_set(occupancies[both], d1) && !check_if_set(occupancies[both], c1) && !check_if_set(occupancies[both], b1)) { // squares between rook and king are not occupied
      // make sure king and the f8 squares are not under attacks
      if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
        add_move(move_list, encode_move(e1, c1, K, 0, 0, 0, 0, 1));
    }
  }

  if ((castle & bk) && color == black) { // king side castling
    if (!check_if_set(occupancies[both], f8) && !check_if_set(occupancies[both], g8)) { // squares between rook and king are not occupied
      // make sure king and the f8 squares are not under attacks
      if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
        add_move(move_list, encode_move(e8, g8, k, 0, 0, 0, 0, 1));
    }
  }

  if ((castle & bq) && color == black) { // queen side castling
    if (!check_if_set(occupancies[both], f8) && !check_if_set(occupancies[both], g8)) { // squares between rook and king are not occupied
      // make sure king and the f8 squares are not under attacks
      if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
        add_move(move_list, encode_move(e8, c8, k, 0, 0, 0, 0, 1));
    }
  }
  
  int piece = (color==white ? K : k);

  while (bitboard != 0ULL) {
    source_square = bitScanForward(bitboard);
    target_squares = king_attacks[source_square] & ((color == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (color == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else {
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }
      remove_bit(&target_squares, target_square);
    }

    remove_bit(&bitboard, source_square);

  }

}


void move_generator(MoveList *move_list) {

  U64 bitboard, attacks;
  move_list->move_count=0;

  
  for (int piece=P; piece <= K; piece++) {
    // bitboard = bitboards[piece]; // copy of the current bitbaord

    
    if (piece==P || piece==p) { // white or black pawns
      to_move==white ? generate_pawn_moves(bitboards[P], white, move_list) : generate_pawn_moves(bitboards[p], black, move_list);
    }  else if (piece==B || piece==b) {
       to_move==white ? genereate_bishop_moves(bitboards[B], white, move_list) : genereate_bishop_moves(bitboards[b], black, move_list);
    } else if (piece==R || piece==r) {
      to_move==white ? generate_rook_moves(bitboards[R], white, move_list) : generate_rook_moves(bitboards[r], black, move_list);
    } else if (piece==N || piece==n) {
      to_move==white ? generate_knight_moves(bitboards[N], white, move_list) : generate_knight_moves(bitboards[n], black, move_list);
    } else if (piece==Q || piece==q) {
      to_move==white ? generate_queen_moves(bitboards[Q], white, move_list) : generate_queen_moves(bitboards[q], black, move_list);
    } else if (piece==K || piece==k) {
      to_move==white ? generate_king_moves(bitboards[K], white, move_list) : generate_king_moves(bitboards[k], black, move_list);
    }
  }
}



int main(int argc, char *argv[]) {



  U64 white_pawns = 0ULL;
  U64 empty = 0ULL;
  

  MoveList move_list;


  init_sliders_attacks(1); // bishop
  init_sliders_attacks(0); // rook
  init_leaping_pieces_attacks();

    
  fen_parser(tricky_position);
  // fen_parser("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPpP/R3K2R b KQkq a3 0 1 ");
  

  move_generator(&move_list);
  print_board();


  print_move_list(move_list);




  return 0;
}