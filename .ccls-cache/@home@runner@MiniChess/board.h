#pragma once

#include <stdint.h>

const char EMPTY = '.';
const char WHITE_PAWN = 'P';
const char BLACK_PAWN = 'p';
const char WHITE_KNIGHT = 'N';
const char BLACK_KNIGHT = 'n';
const char WHITE_BISHOP = 'B';
const char BLACK_BISHOP = 'b';
const char WHITE_ROOK = 'R';
const char BLACK_ROOK = 'r';
const char WHITE_QUEEN = 'Q';
const char BLACK_QUEEN = 'q';
const char WHITE_KING = 'K';
const char BLACK_KING = 'k';

class Piece {
  public:
    Piece();
    Piece(char type);

    char type;
};

class Board {
  public:
    Board();

    void printBoard(bool pretty = false);

  private:
    Piece board[8][8];

    void printPrettyPiece(char algebraic);
};
