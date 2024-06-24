## BOARD REPRESENTATION
- Use of bitboard
- Each bit of a 64 bit word is associated with a square of the chessboard.
- Bit 0 -> square a1, bit7 -> square h1 ...
- Each chess piece type in 64 bits -> 12 variables to represent the board (one black and one white for 
each type)

# Leaping pieces: 
- Pawn
- Knight
- King

# Sliding pieces: 
- Queen
- Rook
- Bishop