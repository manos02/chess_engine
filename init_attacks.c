#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

// unicode pieces
char *unicode_pieces[12] = {"♟︎", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔",};


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


//  block_mask = bitboard containing all squares that can block a piece


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
  U64 key = (blockers * BISHOP_MAGICS[pos]) >> (64 - BISHOP_INDEX_BITS[pos]);
  queen_attacks = bishop_attacks[pos][key];

  rook_blockers &= ROOK_MASKS[pos];
  key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
  queen_attacks |= rook_attacks[pos][key];

  return queen_attacks;
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

      U64 attack_mask = is_bishop == 1 ? ROOK_MASKS[pos] : BISHOP_MASKS[pos];
      int relevant_bits = is_bishop == 1 ? ROOK_INDEX_BITS[pos] : BISHOP_INDEX_BITS[pos];
    
      
      for (int i=0; i < (1<<relevant_bits); i++) { // 2^relevant_bits blocker boards
        
        U64 blockers = generate_blocker_boards(i, attack_mask);
        
        if (is_bishop) {
          U64 key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
          bishop_attacks[pos][key] = generate_bishop_legal_bitboard(blockers, col, row);
        } else {
          U64 key = (blockers * ROOK_MAGICS[pos]) >> (64 - ROOK_INDEX_BITS[pos]);
          rook_attacks[pos][key] = generate_rook_legal_bitboard(blockers, col, row);
        }
      }
    }
  }
}


// U64 is_square_attacked(U64 occupied, int pos, int color) {
  
//   if ((color == white && (pawn_attacks[black][pos]  & bitboards[P]))) // & white pawns
//   if ((color == black && (pawn_attacks[white][pos]   ))) // & black pawns

// }


void fen_parser(char *fen) {

  
  memset(bitboards, 0ULL, sizeof(bitboards)); // initialise bitboards
  memset(occupancies, 0ULL, sizeof(occupancies)); // initialise occupancies

  int pos = 0;
  
  for (int i = 0; i < strlen(fen); i++) {
        
      // int pos = row*8 + col;

    // printf("pos: %d\n", pos);
    // printf("%c\n", fen[i]);


    if ((fen[i] >= 'a' && fen[i] <= 'z') || (fen[i] >= 'A' && fen[i] <= 'Z')) { // check if it is a piece 
        
      // init piece type
      int piece = char_pieces[fen[i]];
      
      // set piece on corresponding bitboard
      set_bit(&bitboards[piece], pos);
      

    // increment pointer
      
      pos++;
    } else if (fen[i] >= '0' && fen[i] <= '9') {
        // for (int bitboard=P; bitboard<k; bitboard++) { // P -> white pawns, k -> black king
          

          // fen++;
        // }
        int offset = fen[i] - '0'; // convert char to int
        // printf("offset: %d\n", offset);
        pos += offset;

    }       
    
  }
       
}

void print_board() {

  
    printf("  +-----------------+\n");
    for (int row=0;row<8;row++)  {
      printf("%d | ", 8 - row);
      for (int col=0;col<8;col++) {
        int pos = row*8+col;

        int piece_at_pos = -1;
        for (int piece=P; piece < k; piece++) {
          if (check_if_set(bitboards[piece], pos)) {
            piece_at_pos = piece;

            // printf("pos: %d %d\n",pos, piece_at_pos);
            // print_bitboard(bitboards[piece_at_pos]);
            break;
          }
        }
        printf("%s ", (piece_at_pos == -1) ? "." : unicode_pieces[piece_at_pos]);
       
        // printf("%llu ", index);
      }

      printf("| %d\n", 8-row);
    }
    printf("  +-----------------+\n");
    printf("    a b c d e f g h\n");
  



}


int main(int argc, char *argv[]) {



  U64 white_pawns = 0ULL;
  U64 empty = 0ULL;


  // init_sliders_attacks(1); // bishop
  // init_sliders_attacks(0); // rook

  // init_leaping_pieces_attacks();
  // // init_sliders_attacks();
  
  // set_bit(&white_pawns, b2);
  // print_bitboard(white_pawns);
  
  
  // set_bit(&empty, a3);
  // set_bit(&empty, b3);
  // set_bit(&empty, c3);
  // set_bit(&empty, a4);
  // set_bit(&empty, b4);
  // set_bit(&empty, c4);

  fen_parser(tricky_position);
  print_board();
  


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