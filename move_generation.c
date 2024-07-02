#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "init_attacks.h"
#include "vars.h"
#include "move_generation.h"


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




/*
                           castling   move     in      in
                              right update     binary  decimal

 king & rooks didn't move:     1111 & 1111  =  1111    15

        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13
     
         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7

*/

// castling rights update constants
const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};


int make_move(U64 move, int move_flag) {


  if (move_flag==all_moves) { // quiet moves

    copy_board();

    int source_square = decode_source_square(move);
    int target_square = decode_target_square(move);
    int piece = decode_piece(move);
    int promoted_piece = decode_promoted_piece(move);
    int capture = decode_capture_flag(move);
    int double_push = decode_dp_flag(move);
    int enpass = decode_enpassant_flag(move);
    int castling = decode_castling_flag(move);

    if (capture) { // capture move
      for (int piece=P; piece <= k; piece++) {
        if (check_if_set(bitboards[piece], target_square)) { // find the correct piece
          remove_bit(&bitboards[piece], target_square);
          break;
        }
      }
    }
    
    remove_bit(&bitboards[piece], source_square); // remove source piece from bitboard

    if (promoted_piece) {
      set_bit(&bitboards[promoted_piece], target_square); // move piece to target square and change it to promoted piece
    }

    if (enpass) {
      int piece = to_move==white ? p : P;
      int before_square = to_move == white ? (target_square + 8) : (target_square - 8); // remove pawn in behind square 
      remove_bit(&bitboards[piece], before_square); 
    }

    if (double_push) {
      enpassant = to_move == white ? (target_square + 8) : (target_square - 8); // set enpessant square the behind square 
    }

    if (castling) {
      if (target_square-source_square == 2) { // king side castling
        if (to_move==white) {
          remove_bit(&bitboards[R], h1);
          set_bit(&bitboards[R], f1);
        } else {
          remove_bit(&bitboards[r], h8);
          set_bit(&bitboards[r], f8);
        } 
      } else { // queen side castling
          if (to_move==white) {
          remove_bit(&bitboards[R], a1);
          set_bit(&bitboards[R], d1);
        } else {
          remove_bit(&bitboards[r], a8);
          set_bit(&bitboards[r], d8);
        } 
      }
    }

    if (!promoted_piece) {
      set_bit(&bitboards[piece], target_square); // move piece to target square
    }

    // update castling rights
    castle &= castling_rights[source_square];
    castle &= castling_rights[target_square];

    // update occupancies
    occupancies[black] = 0ULL;
    occupancies[white] = 0ULL;
    occupancies[both] = 0ULL;

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

    // change side 
    to_move ^= 1; // XOR OPERATOR  


    // king not in check

    if (is_square_attacked(to_move==white ? bitScanForward(bitboards[k]) : bitScanForward(bitboards[K]), to_move)) {
      take_back(); // illegal move
      return 0;
    } else {
      return 1;
    } 
  }

  return 0;


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
        add_move(move_list, encode_move(source_square, target_enpassant, piece,0, 0, 0, 1, 0));

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


  move_list->move_count=0;
  
  for (int piece=P; piece <= K; piece++) {
    
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