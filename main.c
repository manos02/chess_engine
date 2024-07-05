#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "vars.h"
#include "move_generation.h"
#include "init_attacks.h"

#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "

// get time in milliseconds
int get_time_ms() {  
  struct timeval time_value;
  gettimeofday(&time_value, NULL);
  return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
    
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

long nodes;

int count=0;

void perft(int depth) {
  
  MoveList move_list;
  
  
  if (depth == 0) {
    nodes++;
    return;
  }

  move_generator(&move_list);
  

  // loop over generated moves
  for (int move_count = 0; move_count < move_list.move_count; move_count++) {
      
    // preserve board state
    copy_board();
    
    // make move
    if (!make_move(move_list.moves[move_count], all_moves))
      // skip to the next move
      continue;
    // make_move(move_list.moves[move_count], all_moves);


    perft(depth-1);
    
    take_back();
  }

}




int main(int argc, char *argv[]) {


  
  init_all();

    
  fen_parser(tricky_position);
  // fen_parser("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBqPPP/R3K2R w KQkq - 0 1 ");
  

  print_board();


  int start = get_time_ms(); 


  perft(3);

  printf("time taken to execute: %d ms\n", get_time_ms() - start);
  printf("%ld\n", nodes);
  

  
	return 0;
}

