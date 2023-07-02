// https://www.chessprogramming.org/Getting_Started
// https://alexanderameye.github.io/notes/chess-engine/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "board.h"

int main() {
  Board b = Board();
  // b.performMove(b.legalWhiteMoves()[2]);
  // b.performMove(b.legalWhiteMoves()[15]);
  // b.performMove(b.legalWhiteMoves()[18]);
  // std::vector<uint16_t> moves = b.legalBlackMoves();
  // printf("%d\n", moves[moves.size()-1]);
  // b.performMove(moves[moves.size()-1]);
  // b.unMakeMove();
  // b.performMove(b.getLegalBlackPawnMoves()[13]);
  // b.performMove(b.getLegalWhitePawnMoves()[15]);
  // b.performMove(b.getLegalWhitePawnMoves()[14]);
  // b.performMove(b.getLegalWhitePawnMoves()[14]);
  // b.performMove(b.getLegalWhitePawnMoves()[17]);
  // b.unMakeMove();
  printf("Size of board in memory is %d bytes\n", sizeof(b));
  b.printBoard();
  return 0;
}
