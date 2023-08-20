// https://www.chessprogramming.org/Getting_Started
// https://alexanderameye.github.io/notes/chess-engine/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "board.h"
#include "evaluation.h"
#include <bitset>

int main() {
  Board b = Board();
  printf("Negamax: %d\n", negaMax(b, -10000, 10000, 3, 1));
  //b.printBitBoard(b.whitePawns);
  printf("Size of board in memory is %d bytes\n", sizeof(b));
  b.printBoard();
  return 0;
}
