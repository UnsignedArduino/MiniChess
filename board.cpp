#include "board.h"
#include <string.h>
#include <stdio.h>

Board::Board() {
  this->whitePawns = 0x000000000000FF00;
  this->blackPawns = 0x00FF000000000000;
  this->whiteKnights = 0x0000000000000042;
  this->blackKnights = 0x4200000000000000;
  this->whiteBishops = 0;
  this->blackBishops = 0;
  this->whiteRooks = 0;
  this->blackRooks = 0;
  this->whiteQueens = 0;
  this->blackQueens = 0;
  this->whiteKing = 0;
  this->blackKing = 0;
}

void Board::printBoard() {
  for (uint8_t y = 0; y < 8; y ++) {
    for (uint8_t x = 0; x < 8; x ++) {
      const uint8_t index = y * 8 + x;
      if (bitRead(this->whitePawns, index)) {
        printf("P");
      } else if (bitRead(this->blackPawns, index)) {
        printf("p");
      } else if (bitRead(this->whiteKnights, index)) {
        printf("N");
      } else if (bitRead(this->blackKnights, index)) {
        printf("n");
      } else if (bitRead(this->whiteBishops, index)) {
        printf("B");
      } else if (bitRead(this->blackBishops, index)) {
        printf("b");
      } else if (bitRead(this->whiteRooks, index)) {
        printf("R");
      } else if (bitRead(this->blackRooks, index)) {
        printf("r");
      } else if (bitRead(this->whiteQueens, index)) {
        printf("Q");
      } else if (bitRead(this->blackQueens, index)) {
        printf("q");
      } else if (bitRead(this->whiteKing, index)) {
        printf("K");
      } else if (bitRead(this->blackKing, index)) {
        printf("k");
      } else {
        printf(".");
      }
      printf(" ");
    }
    printf("\n");
  }
}