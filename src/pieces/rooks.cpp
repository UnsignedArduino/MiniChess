#include "../board.h"
#include <bits/stdc++.h>
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

std::vector<uint16_t> Board::getLegalWhiteRookMoves() {
  std::vector<uint16_t> legal;

  uint64_t whiteRookCpy = whiteRooks;

  while (whiteRookCpy != 0){
    uint64_t isolatedRook = whiteRookCpy & ((~whiteRookCpy) + 1);
    uint8_t from = 63 - __builtin_ctzll(isolatedRook);

    uint64_t attacks = getFileAttacks(isolatedRook, from) | getRankAttacks(isolatedRook);

    while (attacks != 0){
      uint64_t singleAttackSquare = attacks & ((~attacks) + 1);
      if (!whitePieceAtBitBoard(singleAttackSquare)){
        uint16_t move = movePack(from, 63-__builtin_ctzll(singleAttackSquare), false, false, 0, blackPieceAtBitBoard(singleAttackSquare), 0);
        // std::bitset<16> x(move);
        // std::cout << x << '\n';
        legal.push_back(move);
      }
      attacks &= ~singleAttackSquare;
    }
    whiteRookCpy &= ~isolatedRook;
  }

  printf("%d\n", legal.size());
  return legal;
}