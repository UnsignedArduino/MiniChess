#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

std::vector<uint16_t> Board::getLegalWhitePawnMoves() {
  std::vector<uint16_t> legal;
  // Get pawn pushes first (single)
  // First copy the white pawn BB
  uint64_t whitePawnCpy = this->whitePawns;
  // Isolated pawn BB
  // Set all bits to 0 besides the LS 1
  while (whitePawnCpy != 0){
    uint64_t isolatedPawn = whitePawnCpy & ((~whitePawnCpy) + 1); // Black Magic;
  
    // Check through all the other BBs and make sure no piece is in
    // front of this pawn
  
    // First shift this bit backwards by 8
    const uint64_t possiblePawnPush = isolatedPawn << 8;
    const uint64_t possibleExtraPawnPush = possiblePawnPush << 8;
  
    const uint8_t from = 63 - std::log2(isolatedPawn);
    // Use & to see if any other BB has a 1 there
    if (!this->pieceAtBitBoard(possiblePawnPush)) {
      // Step 1: Find index of origin square (isolatedPawn)
  
      // Step 2: Find index of landing square (possiblePawnPush)
      const uint8_t to = 63 - std::log2(possiblePawnPush);
      // Step 3: Check if it can promote
      const bool canPromote = to < 8;
      if (canPromote) {
        uint16_t move;
        for (uint8_t i = 0; i < 4; i++) {
          moveSet(move, from, to, i);
          // std::bitset<16> x(move);
          // std::cout << x << '\n';
          legal.push_back(move);
        }
        // Step 4: Check if we can jump 2 squares
      } else if (from >= 48 && from <= 55 &&
                 !this->pieceAtBitBoard(possibleExtraPawnPush)) {
        const uint8_t extraTo = 63 - std::log2(possibleExtraPawnPush);
        legal.push_back(movePack(from, to, 0));
        legal.push_back(movePack(from, extraTo, 0));
        // Otherwise can only move up 1 square
      } else {
        legal.push_back(movePack(from, to, 0));
      }
    }
  
    // Next check for captures
    const uint64_t possibleNorthWestCapture = isolatedPawn << 9;
    const uint64_t possibleNorthEastCapture = isolatedPawn << 7;
  
    uint8_t toWest = 63 - std::log2(possibleNorthWestCapture);
    uint8_t toEast = 63 - std::log2(possibleNorthEastCapture);
    if (this->blackPieceAtBitBoard(possibleNorthWestCapture) && (from) % 8 != 0) {
      const bool canPromote = toWest < 8;
      
      if (canPromote) {
        uint16_t move;
        for (uint8_t i = 0; i < 4; i++) {
          moveSet(move, from, toWest, i);
          // std::bitset<16> x(move);
          // std::cout << x << '\n';
          legal.push_back(move);
        }
      } else {
        uint16_t move;
        moveSet(move, from, toWest, 0);
        legal.push_back(move);
      }
    }
    if (this->blackPieceAtBitBoard(possibleNorthEastCapture) && (from + 1) % 8 != 0) {
      const bool canPromote = toEast < 8;
      
      if (canPromote) {
        uint16_t move;
        for (uint8_t i = 0; i < 4; i++) {
          moveSet(move, from, toEast, i);
          // std::bitset<16> x(move);
          // std::cout << x << '\n';
          legal.push_back(move);
        }
      } else {
        uint16_t move;
        moveSet(move, from, toEast, 0);
        legal.push_back(move);
      }
    }
    //Remove isolated bit from the copy
    whitePawnCpy = whitePawnCpy & (~isolatedPawn);
    
  }

  printf("%d\n", legal.size());
  // TODO: EN PASSANT

  return legal;
}
