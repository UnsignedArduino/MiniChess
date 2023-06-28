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
  printf("Size of board in memory is %d bytes\n", sizeof(b));
  b.printBoard();
  return 0;
}
