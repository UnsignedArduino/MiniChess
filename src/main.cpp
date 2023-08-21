// https://www.chessprogramming.org/Getting_Started
// https://alexanderameye.github.io/notes/chess-engine/

#include "board.h"
#include "evaluation.h"
#include <bitset>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  Board b = Board();
  printf("Negamax: %d\n", alphaBeta(b, -10000, 10000, 4));
  //b.printBitBoard(b.whitePawns);
  printf("Size of board in memory is %d bytes\n", sizeof(b));
  // b.performMove(b.legalWhiteMoves()[0]);
  // b.performMove(b.legalBlackMoves()[0]);
  // b.performMove(b.legalWhiteMoves()[0]);
  // b.printBoard();
  // std::vector<uint16_t> moves = b.legalBlackMoves();
  // for (int i=0;i<moves.size();i++){
  //   std::bitset<16> a = moves[i];
  //   std::cout << a << "\n";
  // }
  // b.performMove(b.legalBlackMoves()[2]);
  // printf("Eval: %d\n", evaluate(b));
  b.printBoard();
  return 0;
}
