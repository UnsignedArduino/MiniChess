#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

std::vector<uint16_t> Board::getLegalWhiteKnightMoves() {
  std::vector<uint16_t> legal;

  uint64_t whiteKnightsCpy = whiteKnights;

  while (whiteKnightsCpy != 0) {

    uint64_t isolatedKnight = whiteKnightsCpy & ((~whiteKnightsCpy) + 1);
    uint8_t from = 63 - __builtin_ctzll(isolatedKnight);

    uint64_t attacks = 0x0ULL;
    // Not A file
    if (from % 8 != 0) {
      attacks |= isolatedKnight << 17;
      attacks |= isolatedKnight >> 15;
      // Not A or B file
      if ((from - 1) % 8 != 0) {
        attacks |= isolatedKnight << 10;
        attacks |= isolatedKnight >> 6;
      }
    }

    // Not H File
    if ((from + 1) % 8 != 0) {
      attacks |= isolatedKnight << 15;
      attacks |= isolatedKnight >> 17;
      // Not H or F File
      if ((from + 2) % 8 != 0) {
        attacks |= isolatedKnight << 6;
        attacks |= isolatedKnight >> 10;
      }
    }

    while (attacks != 0) {
      uint64_t singleAttackSquare = attacks & ((~attacks) + 1);
      if (!whitePieceAtBitBoard(singleAttackSquare)) {
        uint16_t move = movePack(from, 63 - __builtin_ctzll(singleAttackSquare), false, false, 0,
                                 blackPieceAtBitBoard(singleAttackSquare), 0);
        // std::bitset<16> x(move);
        // std::cout << x << '\n';
        legal.push_back(move);
      }
      attacks &= ~singleAttackSquare; // 37, 46
    }
    whiteKnightsCpy &= ~isolatedKnight;
  }
  // printf("%d\n", legal.size());
  return legal;
}

std::vector<uint16_t> Board::getLegalBlackKnightMoves() {
  std::vector<uint16_t> legal;

  uint64_t blackKnightsCpy = blackKnights;

  while (blackKnightsCpy != 0) {

    uint64_t isolatedKnight = blackKnightsCpy & ((~blackKnightsCpy) + 1);
    uint8_t from = 63 - __builtin_ctzll(isolatedKnight);

    uint64_t attacks = 0x0ULL;
    // Not A file
    if (from % 8 != 0) {
      attacks |= isolatedKnight << 17;
      attacks |= isolatedKnight >> 15;
      // Not A or B file
      if ((from - 1) % 8 != 0) {
        attacks |= isolatedKnight << 10;
        attacks |= isolatedKnight >> 6;
      }
    }

    // Not H File
    if ((from + 1) % 8 != 0) {
      attacks |= isolatedKnight << 15;
      attacks |= isolatedKnight >> 17;
      // Not H or F File
      if ((from + 2) % 8 != 0) {
        attacks |= isolatedKnight << 6;
        attacks |= isolatedKnight >> 10;
      }
    }

    while (attacks != 0) {
      uint64_t singleAttackSquare = attacks & ((~attacks) + 1);
      if (!blackPieceAtBitBoard(singleAttackSquare)) {
        uint16_t move = movePack(from, 63 - __builtin_ctzll(singleAttackSquare), false, false, 0,
                                 whitePieceAtBitBoard(singleAttackSquare), 0);
        // std::bitset<16> x(move);
        // std::cout << x << '\n';
        legal.push_back(move);
      }
      attacks &= ~singleAttackSquare; // 37, 46
    }
    blackKnightsCpy &= ~isolatedKnight;
  }
  // printf("%d\n", legal.size());
  return legal;
}