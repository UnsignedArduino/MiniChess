#include "board.h"
#include <bitset>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

// Promo: 1 = knight, 2 = bishop, 3 = rook, 4 = queen
// https://www.chessprogramming.org/Encoding_Moves
void moveSet(uint16_t &move, uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture, uint8_t castle) {
  move = (uint16_t)from;
  move |= (uint16_t)to << 6;
  uint8_t flags = 0;
  if (doublePawnPush){
    flags = 0b001;
  }
  else if (castle != 0){
    flags = 0b0010 + (castle - 1);
  }
  else if (capture){
    if (promo != 0){
      flags = 0b1100 + (promo - 1);
    }
    else {
      flags = 0b0100;
    }
  }
  else if (promo != 0){
    flags = 0b1000 + (promo - 1);
  }
  else if (enPassant){
    flags = 0b0101;
  }
  move |= (uint16_t)flags << 12;
  // std::bitset<16> x(move);
  // std::cout << x << '\n';
}

uint16_t movePack(uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture, uint8_t castle) {
  uint16_t move;
  moveSet(move, from, to, doublePawnPush, enPassant, promo, capture, castle);
  return move;
}

Board::Board() {
  this->whitePawns = 0x000000000000FF00ULL;
  this->blackPawns = 0x00FF000000000000ULL;
  this->whiteKnights = 0x0000000000000042ULL;
  this->blackKnights = 0x4200000000000000ULL;
  this->whiteBishops = 0x0000000000000024ULL;
  this->blackBishops = 0x2400000000000000ULL;
  this->whiteRooks = 0x0000000000000081ULL;
  this->blackRooks = 0x8100000000000000ULL;
  this->whiteQueens = 0x0000000000000010ULL;
  this->blackQueens = 0x1000000000000000ULL;
  this->whiteKing = 0x000000000000008ULL;
  this->blackKing = 0x800000000000000ULL;

  // this->blackKnights >>= 32;
  // this->blackKing >>= 24;
  // this->whiteQueens <<= 24;
  // this->whiteBishops = 0x0000000000000024ULL << 24;
  // this->whiteRooks = 0x0000000081000000ULL << 3;
  // this->whitePawns = 0x000000010100FF00ULL;
  // this->blackPawns = 0x0000000000FF0000;
  // this->blackPawns = 0;
  // this->blackRooks = 0;

  // this->getLegalWhiteBishopMoves();
  // this->getLegalBlackQueenMoves();
  // this->getLegalBlackKingMoves();
  // this->getLegalBlackKnightMoves();
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

bool Board::whitePieceAtBitBoard(uint64_t bb) {
  return this->whitePawns & bb || this->whiteKnights & bb ||
         this->whiteBishops & bb || this->whiteRooks & bb ||
         this->whiteQueens & bb || this->whiteKing & bb & bb;
}

bool Board::blackPieceAtBitBoard(uint64_t bb) {
  return this->blackPawns & bb || this->blackKnights & bb ||
         this->blackBishops & bb || this->blackRooks & bb ||
         this->blackQueens & bb || this->blackKing & bb & bb;
}

uint64_t Board::combinedOccupationBitBoard() {
  return this->whitePawns | this->blackPawns | this->whiteKnights |
         this->blackKnights | this->whiteBishops | this->blackBishops |
         this->whiteRooks | this->blackRooks | this->whiteQueens |
         this->blackQueens | this->whiteKing | this->blackKing;
}

uint64_t Board::bitReverse(uint64_t n) {
  short bits = 64;
  uint64_t mask = ~uint64_t(
      0); // equivalent to uint32_t mask = 0b11111111111111111111111111111111;

  while (bits >>= 1) {
    mask ^=
        mask
        << (bits); // will convert mask to 0b00000000000000001111111111111111;
    n = (n & ~mask) >> bits | (n & mask) << bits; // divide and conquer
  }
  return n;
}

uint64_t Board::flipVertical(uint64_t x) {
  return ((x << 56)) | ((x << 40) & (0x00ff000000000000)) |
         ((x << 24) & (0x0000ff0000000000)) |
         ((x << 8) & (0x000000ff00000000)) | ((x >> 8) & (0x00000000ff000000)) |
         ((x >> 24) & (0x0000000000ff0000)) |
         ((x >> 40) & (0x000000000000ff00)) | ((x >> 56));
}

uint64_t Board::rotate90(uint64_t bb) {
  uint64_t t;
  const uint64_t k1 = (0xaa00aa00aa00aa00);
  const uint64_t k2 = (0xcccc0000cccc0000);
  const uint64_t k4 = (0xf0f0f0f00f0f0f0f);
  t = bb ^ (bb << 36);
  bb ^= k4 & (t ^ (bb >> 36));
  t = k2 & (bb ^ (bb << 18));
  bb ^= t ^ (t >> 18);
  t = k1 & (bb ^ (bb << 9));
  bb ^= t ^ (t >> 9);
  return flipVertical(bb);
}

uint64_t Board::rotateAnti90(uint64_t bb) {
  bb = flipVertical(bb);
  uint64_t t;
  const uint64_t k1 = (0xaa00aa00aa00aa00);
  const uint64_t k2 = (0xcccc0000cccc0000);
  const uint64_t k4 = (0xf0f0f0f00f0f0f0f);
  t = bb ^ (bb << 36);
  bb ^= k4 & (t ^ (bb >> 36));
  t = k2 & (bb ^ (bb << 18));
  bb ^= t ^ (t >> 18);
  t = k1 & (bb ^ (bb << 9));
  bb ^= t ^ (t >> 9);
  return bb;
}

uint64_t Board::getFileAttacks(uint64_t bitMask, uint16_t from){
  
  uint64_t fileMaskEx = (0x101010101010101ULL << (uint16_t)(7 - (from % 8))) & (~bitMask);
  uint64_t forward, reverse;
  forward  = combinedOccupationBitBoard() & fileMaskEx;
  reverse  = bitReverse(forward);
  forward -= bitMask;
  reverse -= bitReverse(bitMask);
  forward ^= bitReverse(reverse);
  forward &= fileMaskEx;
  return forward;
}

uint64_t Board::getRankAttacks(uint64_t bitMask){

  bitMask = rotate90(bitMask);

  uint64_t fileMaskEx = (0x101010101010101ULL << (uint16_t)(7 - ( (63 - __builtin_ctzll(bitMask)) % 8))) & (~bitMask);
  uint64_t forward, reverse;
  forward  = rotate90(combinedOccupationBitBoard()) & fileMaskEx;
  reverse  = bitReverse(forward);
  forward -= bitMask;
  reverse -= bitReverse(bitMask);
  forward ^= bitReverse(reverse);
  forward &= fileMaskEx;
  return rotateAnti90(forward);
}

uint64_t Board::getDiagonalAttacks(uint64_t bitMask, uint16_t from){
  uint64_t diagonalMaskEx;
  int diag = 7-((63 - from) & 7) - ((63 - from) >> 3);
  diagonalMaskEx = diag >= 0 ? 0x0102040810204080ULL >> diag*8 : 0x0102040810204080ULL << -diag*8;
  diagonalMaskEx &= ~bitMask;

  uint64_t antidiagMaskEx;
  diag = ((63 - from) & 7) - ((63 - from) >> 3);
  antidiagMaskEx = diag >= 0 ? 0x8040201008040201ULL >> diag*8 : 0x8040201008040201ULL << -diag*8;
  antidiagMaskEx &= ~bitMask;

  uint64_t occ = combinedOccupationBitBoard();
  // Positive Diagonal attacks
  uint64_t forward, reverse;
  forward = occ & diagonalMaskEx;
  reverse  = bitReverse(forward);
  forward -= bitMask;
  reverse -= bitReverse(bitMask);
  forward ^= bitReverse(reverse);
  forward &= diagonalMaskEx;
  // Anti Diagonal Attacks
  uint64_t forward2, reverse2;
  forward2  = occ & antidiagMaskEx;
  reverse2  = bitReverse(forward2);
  forward2 -= bitMask;
  reverse2 -= bitReverse(bitMask);
  forward2 ^= bitReverse(reverse2);
  forward2 &= antidiagMaskEx;
  return forward + forward2;
}

void Board::printBitBoard(uint64_t bb) {
  for (uint8_t y = 0; y < 8; y++) {
    for (uint8_t x = 0; x < 8; x++) {
      const uint8_t index = y * 8 + x;
      printf("%d", bitRead(bb, index));
    }
    printf("\n");
  }
  printf("\n");
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
