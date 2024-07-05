#ifndef VARS_H
#define VARS_H

#include <stdint.h>

typedef unsigned long long U64;

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

enum {white, black, both};
enum {P, N, B, R, Q, K, p, n, b, r, q, k};
enum { wk = 1, wq = 2, bk = 4, bq = 8 }; // castling
enum {all_moves, capture_moves};

typedef struct MoveList {
  int move_count;
  int moves[254];
} MoveList;

// piece bitboards
extern U64 bitboards[12];

// occupancy bitboards
extern U64 occupancies[3];

// blocker masks 
extern U64 BISHOP_MASKS[64];
extern U64 ROOK_MASKS[64];
extern U64 QUEEN_MASKS[64];

// attack tables
extern U64 pawn_attacks[2][64];
extern U64 knight_attacks[64];
extern U64 king_attacks[64];
extern U64 bishop_attacks[64][1024];
extern U64 rook_attacks[64][4096];

extern const U64 ROOK_MAGICS[64];
extern const U64 BISHOP_MAGICS[64];

// bits of blocker mask 
extern const int ROOK_INDEX_BITS[64];
extern const int BISHOP_INDEX_BITS[64];

// rows and cols
extern const U64 not_a_col;
extern const U64 not_h_col;
extern const U64 not_ab_col;
extern const U64 not_gh_col;
extern const U64 not_8_row;
extern const U64 not_87_row;
extern const U64 not_1_row;
extern const U64 not_12_row;
extern const U64 row_4;
extern const U64 row_5;

// convert ASCII character pieces to encoded constants
extern int char_pieces[];

// promoted pieces
extern char promoted_pieces[];

// unicode pieces
extern char *unicode_pieces[12];

extern const int lsb_64_table[64];

// convert squares to coordinates
extern const char *square_to_coordinates[];

// side to move
extern int to_move;

// enpassant square
extern int enpassant; 

// castling rights
extern int castle;

extern int halfmove_clock; // the number of halfmoves since the last capture or pawn advance used for the fifty rule

extern int fullmove_num; // the number of full moves 

#endif