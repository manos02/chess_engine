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
   
  if (bb) {
    unsigned int folded;
    bb ^= bb - 1;
    folded = (int) bb ^ (bb >> 32);
    return lsb_64_table[folded * 0x78291ACF >> 26];
  } else {
    return -1;
  }
}


int encode_move(int source, int target, int piece, int promoted, int capture, int doubl, int en, int castling) {

  int move=0;

  move |= source;
  move |= (target << 6);
  move |= (piece << 12);
  move |= (promoted << 16);
  move |= (capture << 20);
  move |= (doubl << 21);
  move |= (en << 22);
  move |= (castling << 23);
  
  return move;
}

int decode_source_square(int move) {
  return move & 0x3f;
}

int decode_target_square(int move) {
  return (move & 0xfc0) >> 6;
}

int decode_piece(int move) {
  return (move & 0xf000) >> 12;
  
}

int decode_promoted_piece(int move) {
  return (move & 0xf0000) >> 16;
}

int decode_capture_flag(int move) {
  return (move & 0x100000);
}

int decode_dp_flag(int move) {
  return (move & 0x200000);
}

int decode_enpassant_flag(int move) {
  return (move & 0x400000);
  
}

int decode_castling_flag(int move) {
  return (move & 0x800000);
}


void add_move(MoveList *move_list, int move) {
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

    int move = move_list.moves[i]; // get move
    

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



int make_move(int move, int move_flag) {

  // printf("%d ", move);
  

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
    
    // printf("%s -> %s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
    

    remove_bit(&bitboards[piece], source_square); // remove source piece from bitboard
    set_bit(&bitboards[piece], target_square); // move piece to target square
    

    if (capture) { // capture move
      int start_piece, end_piece;

      start_piece = to_move==white ? p : P;
      end_piece = to_move==white ? k : K;
      for (int piece=start_piece; piece <= end_piece; piece++) {
        if (check_if_set(bitboards[piece], target_square)) { // find the correct piece
          remove_bit(&bitboards[piece], target_square);
          break;
        }
      }
    }
    

    if (promoted_piece) {
      printf("%s -> %s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);

      remove_bit(&bitboards[piece], target_square); // remove source piece from bitboard
      set_bit(&bitboards[promoted_piece], target_square); // move piece to target square and change it to promoted piece
    }


    if (enpass) {
      int piece = to_move==white ? p : P;
      int before_square = to_move == white ? (target_square + 8) : (target_square - 8); // remove pawn in behind square 
      remove_bit(&bitboards[piece], before_square); 
    }

    enpassant = -1;

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



    // update castling rights
    castle &= castling_rights[source_square];
    castle &= castling_rights[target_square];

    // update occupancies
    occupancies[black] = 0ULL;
    occupancies[white] = 0ULL;
    occupancies[both] = 0ULL;

    for (int bb_piece = P; bb_piece <= K; bb_piece++)
      // update white occupancies
      occupancies[white] |= bitboards[bb_piece];

    // loop over black pieces bitboards
    for (int bb_piece = p; bb_piece <= k; bb_piece++)
      // update black occupancies
      occupancies[black] |= bitboards[bb_piece];

    occupancies[both] |= occupancies[white]; 
    occupancies[both] |= occupancies[black]; 

    // change side 
    to_move ^= 1; // XOR OPERATOR  


    // king not in check

    if (is_square_attacked(to_move==white ? bitScanForward(bitboards[k]) : bitScanForward(bitboards[K]), to_move)) {
      take_back(); 
      return 0; // illegal move
    } else {
      return 1; // legal move
    } 
  }
}

void generate_pawn_moves(U64 bitboard, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks;
  int piece = (to_move==white ? P : p);

  while (bitboard) {
    
    // init source square
    source_square = bitScanForward(bitboard);
                  
    // init target square
    target_square = to_move == white ? (source_square - 8) : (source_square + 8); // - 8 for white + 8 for black
    int upper_boundary = to_move == white ? (target_square >= a8) : (target_square <= h1); // upper boundary 0 violates rule


    if (upper_boundary && !check_if_set(occupancies[both], target_square)) { // check boundary and no other piece at target
      if (source_square >= (to_move == white ? a7 : a2) && source_square <= (to_move == white ? h7 : h2)) { // pawn upgrade
        add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? Q : q), 0, 0, 0, 0));
        add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? N : n), 0, 0, 0, 0));
        add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? B : b), 0, 0, 0, 0));
        add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? R : r), 0, 0, 0, 0));
      } else { // not a promotion
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

        // two squares ahead pawn move
        int double_pawn_target = to_move==white ? (target_square - 8) : (target_square + 8);
    
        if ((source_square >= (to_move == white ? a2 : a7) && source_square <= (to_move == white ? h2 : h7)) && !check_if_set(occupancies[both], double_pawn_target)) 
          add_move(move_list, encode_move(source_square, double_pawn_target, piece, 0, 0, 1, 0, 0));
      }     
    }

    // init pawn attacks bitboard
  attacks = pawn_attacks[to_move][source_square] & (to_move == white ? occupancies[black] : occupancies[white]);

  while (attacks) {
    target_square = bitScanForward(attacks);

    if (source_square >= (to_move == white ? a7 : a2) && source_square <= (to_move == white ? h7 : h2)) { // pawn upgrade
      add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? Q : q), 1, 0, 0, 0));
      add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? N : n), 1, 0, 0, 0));
      add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? B : b), 1, 0, 0, 0));
      add_move(move_list, encode_move(source_square, target_square, piece, (to_move==white ? R : r), 1, 0, 0, 0));

    } else {
      // one square ahead pawn move
      add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

    }

    remove_bit(&attacks, target_square);
  }

  if (enpassant != -1) {
    U64 enpassant_attacks = pawn_attacks[to_move][source_square] & (1ULL << enpassant); 

    // make sure enpassant capture available
    if (enpassant_attacks) {
        // init enpassant capture target square
        int target_enpassant = bitScanForward(enpassant_attacks);
        add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));

    }
  }

  remove_bit(&bitboard, source_square);
  }

}

void generate_bishop_moves(U64 bitboard, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (to_move==white ? B : b);


  while (bitboard) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite color squares
    target_squares = get_bishop_attacks(source_square, occupancies[both]) & ((to_move == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (to_move == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {

      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) { // capture
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else { // quiet
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }

      remove_bit(&target_squares, target_square);
    }
    remove_bit(&bitboard, source_square);
  }
}

void generate_rook_moves(U64 bitboard, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (to_move==white? R : r);


  while (bitboard != 0ULL) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite color squares
    target_squares = get_rook_attacks(source_square, occupancies[both]) & ((to_move == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (to_move == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) { // capture move
        add_move(move_list, encode_move(target_square, piece, 0, 1, 0, 0, 0, 0));
      } else { // quiet move
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }
      remove_bit(&target_squares, target_square);
    }

    remove_bit(&bitboard, source_square);
  }
}

void generate_knight_moves(U64 bitboard, MoveList *move_list) {
  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (to_move==white ? N : n);


  while (bitboard) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite color squares
    target_squares = knight_attacks[source_square] & ((to_move == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (to_move == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) { // capture move
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else { // quiet move
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }
      remove_bit(&target_squares, target_square);
    }
    remove_bit(&bitboard, source_square);
  }
}

void generate_queen_moves(U64 bitboard, MoveList *move_list) {
  int source_square, target_square;
  U64 attacks, target_squares;
  int piece = (to_move==white ? Q : q);

  while (bitboard) {
    source_square = bitScanForward(bitboard);

    // target empty and opposite to_move squares
    target_squares = get_queen_attacks(source_square, occupancies[both]) & ((to_move == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (to_move == white ? occupancies[black] : occupancies[white]);
    

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) { // capture move
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else { // quiet move
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
      }
      remove_bit(&target_squares, target_square);
    }
    remove_bit(&bitboard, source_square);
  }
}

void generate_king_moves(U64 bitboard, MoveList *move_list) {

  int source_square, target_square;
  U64 attacks, target_squares;

  if ((castle & wk) && to_move == white) { // king side castling
    if (!check_if_set(occupancies[both], f1) && !check_if_set(occupancies[both], g1)) { // squares between rook and king are not occupied
      // make sure king and the f1 squares are not under attacks
      if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
        add_move(move_list, encode_move(e1, g1, K, 0, 0, 0, 0, 1));
    }
  }

  if ((castle & wq) && to_move == white) { // queen side castling
    if (!check_if_set(occupancies[both], d1) && !check_if_set(occupancies[both], c1) && !check_if_set(occupancies[both], b1)) { // squares between rook and king are not occupied
      // make sure king and the f8 squares are not under attacks
      if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
        add_move(move_list, encode_move(e1, c1, K, 0, 0, 0, 0, 1));
    }
  }

  if ((castle & bk) && to_move == black) { // king side castling
    if (!check_if_set(occupancies[both], f8) && !check_if_set(occupancies[both], g8)) { // squares between rook and king are not occupied
      // make sure king and the f8 squares are not under attacks
      if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
        add_move(move_list, encode_move(e8, g8, k, 0, 0, 0, 0, 1));
    }
  }

  if ((castle & bq) && to_move == black) { // queen side castling
    if (!check_if_set(occupancies[both], d8) && !check_if_set(occupancies[both], c8) && !check_if_set(occupancies[both], b8)) { // squares between rook and king are not occupied
      // make sure king and the f8 squares are not under attacks
      if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
        add_move(move_list, encode_move(e8, c8, k, 0, 0, 0, 0, 1));
    }
  }
  
  int piece = (to_move==white ? K : k);

  while (bitboard) {
    source_square = bitScanForward(bitboard);
    target_squares = king_attacks[source_square] & ((to_move == white) ? ~occupancies[white] : ~occupancies[black]);
    attacks = target_squares & (to_move == white ? occupancies[black] : occupancies[white]);

    while (target_squares) {
      target_square = bitScanForward(target_squares);
      if (check_if_set(attacks, target_square)) { // capture move
        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
      } else { // quiet move
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
    
    if (piece==P) { // white or black pawns
      generate_pawn_moves(to_move==white ? bitboards[P] : bitboards[p], move_list);
    }  else if (piece==B) {
       generate_bishop_moves(to_move==white ? bitboards[B] : bitboards[b], move_list);
    } else if (piece==R) {
      generate_rook_moves(to_move==white ? bitboards[R] : bitboards[r], move_list);
    } else if (piece==N) {
      generate_knight_moves(to_move==white ? bitboards[N] : bitboards[n], move_list);
    } else if (piece==Q) {
      generate_queen_moves(to_move==white ? bitboards[Q] : bitboards[q], move_list);
    } else if (piece==K) {
      generate_king_moves(to_move==white ? bitboards[K] : bitboards[k], move_list);
    }
  }
}