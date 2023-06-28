#include "board.h"
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>


void moveSet(uint16_t &move, uint8_t from, uint8_t to, uint8_t promo) {
  move = (uint16_t)from;
  move |= (uint16_t)to << 6;
  move |= (uint16_t)promo << 12;
}

uint16_t movePack(uint8_t from, uint8_t to, uint8_t promo) {
  uint16_t move;
  moveSet(move, from, to, promo);
  return move;
}

Board::Board() {
  this->whitePawns = 0x000000000000FF00;
  this->blackPawns = 0x00FF000000000000;
  this->whiteKnights = 0x0000000000000042;
  this->blackKnights = 0x4200000000000000;
  this->whiteBishops = 0x0000000000000024;
  this->blackBishops = 0x2400000000000000;
  this->whiteRooks = 0x0000000000000081;
  this->blackRooks = 0x8100000000000000;
  this->whiteQueens = 0x0000000000000010;
  this->blackQueens = 0x1000000000000000;
  this->whiteKing = 0x000000000000008;
  this->blackKing = 0x800000000000000;

  this->whitePawns = 0x00FF000000000000;
  this->blackPawns = 0;
  this->blackRooks = 0;
  this->blackKing = 0;

  getLegalWhitePawnMoves();
}

bool Board::pieceAtIndex(uint8_t i) {
  return bitRead(this->whitePawns, i) || bitRead(this->blackPawns, i) ||
         bitRead(this->whiteKnights, i) || bitRead(this->blackKnights, i) ||
         bitRead(this->whiteBishops, i) || bitRead(this->blackBishops, i) ||
         bitRead(this->whiteRooks, i) || bitRead(this->blackRooks, i) ||
         bitRead(this->whiteQueens, i) || bitRead(this->blackQueens, i) ||
         bitRead(this->whiteKing, i) || bitRead(this->blackKing, i);
}

bool Board::pieceAtBitBoard(uint64_t bb) {
  return this->whitePawns & bb || this->blackPawns & bb ||
         this->whiteKnights & bb || this->blackKnights & bb ||
         this->whiteBishops & bb || this->blackBishops & bb ||
         this->whiteRooks & bb || this->blackRooks & bb ||
         this->whiteQueens & bb || this->blackQueens & bb ||
         this->whiteKing & bb || this->blackKing & bb;
}

std::vector<uint16_t> Board::getLegalWhitePawnMoves() {
  std::vector<uint16_t> legal;
  // Get pawn pushes first (single)
  // First copy the white pawn BB
  uint64_t whitePawnCpy = this->whitePawns;
  // Isolated pawn BB
  // Set all bits to 0 besides the LS 1
  uint64_t isolatedPawn = whitePawnCpy & ((~whitePawnCpy) + 1); // Black Magic;

  // Check through all the other BBs and make sure no piece is in
  // front of this pawn

  // First shift this bit backwards by 8...?
  const uint64_t possiblePawnPush = isolatedPawn << 8;

  // Use & to see if any other BB has a 1 there
  if (!this->pieceAtBitBoard(possiblePawnPush)) {
    // Step 1: Find index of origin square (isolatedPawn)
    const uint8_t from = 63 - std::log2(isolatedPawn);
    // Step 2: Find index of landing square (possiblePawnPush)
    const uint8_t to = 63 - std::log2(possiblePawnPush);
    // Step 3: Check if it can promote
    const bool canPromote = to < 8;
    if (canPromote) {
      uint16_t move;
      for (uint8_t i = 0; i < 4; i ++) {
        moveSet(move, from, to, i);
        std::cout << std::format("{:b}", move);
        legal.push_back(move);
      }
    } else {
      legal.push_back(movePack(from, to, 0));
    }
  }
  printf("%d\n", legal.size());
  // 00 00 101111 110111
  // 00 01 101111 110111
  
  // For testing purpose
  // this->whitePawns = possiblePawnPush & blackPawns;

  return legal;
}

void Board::printBoard() {
  for (uint8_t y = 0; y < 8; y++) {
    for (uint8_t x = 0; x < 8; x++) {
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
