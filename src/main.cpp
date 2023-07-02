// https://www.chessprogramming.org/Getting_Started
// https://alexanderameye.github.io/notes/chess-engine/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "board.h"
#include <bitset>

int main() {
  Board b = Board();
  // bool turn = 0;
  // for (int i=0;i<12;i++){
  //   if (turn == 0){
  //     std::vector<uint16_t> moves = b.legalWhiteMoves();
  //     b.performMove(moves[0]);
  //   }
  //   else {
  //     std::vector<uint16_t> moves = b.legalBlackMoves();
  //     b.performMove(moves[0]);
  //   }
  //   turn = !turn;
  //   b.printBoard();
  //   printf("\n\n");
  // }
  // b.performMove(b.legalWhiteMoves()[2]);
  // b.performMove(b.legalWhiteMoves()[15]);
  // b.performMove(b.legalWhiteMoves()[18]);
  // b.performMove(b.legalWhiteMoves()[0]);
  // b.performMove(b.legalBlackMoves()[0]);
  // b.performMove(b.legalWhiteMoves()[0]);
  // b.performMove(b.legalBlackMoves()[0]);
  // std::bitset<16> x(b.legalBlackMoves()[0]);
  // std::cout << x << "\n";
  // std::vector<uint16_t> moves = b.legalBlackMoves();
  // printf("%d\n", moves[0]);
  //b.performMove(moves[0]);
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
