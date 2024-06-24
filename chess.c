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

// TODO: 
U64 BISHOP_TABLE[64][1024];
U64 ROOK_TABLE[64][4096];

// magic numbers
const U64 ROOK_MAGICS[64] = {
  0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
  0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
  0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
  0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
  0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
  0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
  0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
  0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
  0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
  0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
  0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
  0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
  0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const U64 BISHOP_MAGICS[64] = {
  0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
  0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
  0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
  0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
  0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
  0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
  0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
  0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
  0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
  0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
  0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
  0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
  0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL
};

// bits of blocker mask 
const int ROOK_INDEX_BITS[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

const int BISHOP_INDEX_BITS[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

// blocker masks 
U64 BISHOP_MASKS[64];
U64 ROOK_MASKS[64];
U64 QUEEN_MASKS[64];

// rows and cols
const U64 not_a_col = 18374403900871474942ULL;
const U64 not_h_col = 9187201950435737471ULL;
const U64 not_ab_col = 18229723555195321596ULL;
const U64 not_gh_col = 4557430888798830399ULL;
const U64 not_8_row = 18446744073709551360ULL;
const U64 not_87_row = 18446744073709486080ULL;
const U64 not_1_row = 72057594037927935ULL;
const U64 not_12_row = 281474976710655ULL;
const U64 row_4 = 1095216660480ULL;
const U64 row_5 = 4278190080ULL;


// attack tables
U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];


void set_bit(U64 *piece, int pos) {
  (*piece) = (*piece) | (1ULL << pos);
}

void remove_bit(U64 *piece, int pos) {
  if ((*piece) & (1ULL << pos)) // chech if there is a piece to remove
  (*piece) = (*piece) ^ (1ULL << pos);
}

int check_if_set(U64 board, int pos) {
  if (board & (1ULL << pos)) {
    return 1;
  }
  return 0;
}

// // count the number of bits that are 1
// int count_bits(U64 board) {
//   U64 count = 0;
//   while(board) {
//     count += board & 1;
//     board >>= 1;
//   }
//   return count;
// }



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


//  block_mask = bitboard containing all squares that can block a piece


U64 get_bishop_attacks(int pos, U64 blockers) {
  blockers &= BISHOP_MASKS[pos];

  U64 key = (blockers * BISHOP_MAGICS[pos]) >> (64 - BISHOP_INDEX_BITS[pos]);

  return BISHOP_TABLE[pos][key];

}


U64 get_rook_attacks(int pos, U64 blockers) {

  blockers &= ROOK_MASKS[pos];
  U64 key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);

  return ROOK_TABLE[pos][key];
}


void init_leaping_pieces_attacks() {
  for (int i=0; i<64; i++) {
    pawn_attacks[white][i] = generate_pawn_attacks(i, white);
    pawn_attacks[black][i] = generate_pawn_attacks(i, black);
    knight_attacks[i] = generate_knight_attacks(i);
    king_attacks[i] = generate_king_attacks(i);
  }
}



U64 generate_rook_legal_bitboard(U64 blockers, int col, int row) {  

  U64 attack_board = 0ULL;

  for (int temp_col = col + 1; temp_col < 7; temp_col++)  {
    int current = row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers,  current)) {
      break;
    }
  }

  for (int temp_col = col - 1; temp_col > 0; temp_col--)  {
    int current = row * 8 + temp_col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers,  current)) {
      break;
    }
  }

  for (int temp_row = row + 1; temp_row < 7; temp_row++)  {
    int current = temp_row * 8 + col;
    set_bit((&attack_board), current);
    if (check_if_set(blockers,  current)) {
      break;
    }
  }

  for (int temp_row = row - 1; temp_row > 0; temp_row--)  {
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

  int temp_col = col + 1;
  int temp_row = row + 1;
  while (temp_row < 7 && temp_col < 7) {
    set_bit((&attack_board), temp_row * 8 + temp_col);
    temp_col++;
    temp_row++;
  }
  return attack_board;
}


// generate blocker masks for bishop, rooks and queens
void init_sliders_attacks(int is_bishop) {

  for (int row=0; row<8; row++) {
    for (int col=0; col<8; col++) {
      int pos = row * 8 + col;


      BISHOP_MASKS[pos] = mask_bishop(col, row);
      ROOK_MASKS[pos] = mask_rook(col, row);  
      QUEEN_MASKS[pos] = ROOK_MASKS[pos] | BISHOP_MASKS[pos];

      // U64 attack_mask = BISHOP_MASKS[pos];
      U64 attack_mask = is_bishop == 1 ? ROOK_MASKS[pos] : BISHOP_MASKS[pos];
      int relevant_bits = is_bishop == 1 ? ROOK_INDEX_BITS[pos] : BISHOP_INDEX_BITS[pos];
    
      
      for (int i=0; i < (1<<relevant_bits); i++) { // 2^relevant_bits blocker boards
        
        U64 blockers = generate_blocker_boards(i, attack_mask);
        
        if (is_bishop) {
          U64 key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
          BISHOP_TABLE[pos][key] = generate_bishop_legal_bitboard(blockers, col, row);
        } else {
          U64 key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
          ROOK_TABLE[pos][key] = generate_rook_legal_bitboard(blockers, col, row);
        }
      }
    }
  }
}


// U64 is_square_attacked(U64 occupied, int pos, int color) {
  
//   if ((color == white && (pawn_attacks[black][pos]   ))) // & white pawns

//   if ((color == black && (pawn_attacks[white][pos]   ))) // & black pawns



// }



int main(int argc, char *argv[]) {



  U64 white_pawns = 0ULL;
  U64 empty = 0ULL;


  init_sliders_attacks(1); // bishop
  init_sliders_attacks(0); // rook

  

  
  // init_sliders_attacks();
  
  // set_bit(&white_pawns, b2);
  
  
  set_bit(&empty, a3);
  set_bit(&empty, b3);
  set_bit(&empty, c3);
  set_bit(&empty, a4);
  set_bit(&empty, b4);
  set_bit(&empty, c4);

 


  // print_board(wDblPushTargets(white_pawns, empty, white));
  // init_leaping_pieces_attacks();


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