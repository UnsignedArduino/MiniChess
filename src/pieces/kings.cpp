#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

std::vector<uint16_t> Board::getLegalWhiteKingMoves() {
  std::vector<uint16_t> legal;

  uint64_t whiteKingCpy = whiteKing;

  uint64_t isolatedKing = whiteKingCpy & ((~whiteKingCpy) + 1);

  uint8_t from = 63 - __builtin_ctzll(isolatedKing);

  uint64_t attacks = isolatedKing << 8;
  attacks |= isolatedKing >> 8;
  attacks |= isolatedKing << 7;
  attacks |= isolatedKing << 9;
  attacks |= isolatedKing >> 7;
  attacks |= isolatedKing >> 9;
  attacks |= isolatedKing << 1;
  attacks |= isolatedKing >> 1;
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

  // printf("%d\n", legal.size());
  return legal;
}

std::vector<uint16_t> Board::getLegalBlackKingMoves() {
  std::vector<uint16_t> legal;

  uint64_t blackKingCpy = blackKing;

  uint64_t isolatedKing = blackKingCpy & ((~blackKingCpy) + 1);

  uint8_t from = 63 - __builtin_ctzll(isolatedKing);

  uint64_t attacks = isolatedKing << 8;
  attacks |= isolatedKing >> 8;
  attacks |= isolatedKing << 7;
  attacks |= isolatedKing << 9;
  attacks |= isolatedKing >> 7;
  attacks |= isolatedKing >> 9;
  attacks |= isolatedKing << 1;
  attacks |= isolatedKing >> 1;
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

  // printf("%d\n", legal.size());
  return legal;
}