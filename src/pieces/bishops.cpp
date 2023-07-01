#include "../board.h"
#include <bits/stdc++.h>
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

std::vector<uint16_t> Board::getLegalWhiteBishopMoves(){
    std::vector<uint16_t> legal;

    uint64_t whiteBishopsCpy = whiteBishops;

    while (whiteBishopsCpy != 0){
        uint64_t isolatedBishop = whiteBishopsCpy & ((~whiteBishopsCpy) + 1);
        uint8_t from = 63 - __builtin_ctzll(isolatedBishop);

        uint64_t attacks = getDiagonalAttacks(isolatedBishop, from);
        while (attacks != 0){
        uint64_t singleAttackSquare = attacks & ((~attacks) + 1);
        if (!whitePieceAtBitBoard(singleAttackSquare)){
            uint16_t move = movePack(from, 63-__builtin_ctzll(singleAttackSquare), false, false, 0, blackPieceAtBitBoard(singleAttackSquare), 0);
            // std::bitset<16> x(move);
            // std::cout << x << '\n';
            legal.push_back(move);
        }
        attacks &= ~singleAttackSquare; //37, 46
        }
        whiteBishopsCpy &= ~isolatedBishop;
    }
  
    // printf("%d\n", legal.size());
}

std::vector<uint16_t> Board::getLegalBlackBishopMoves(){
    std::vector<uint16_t> legal;

    uint64_t blackWhiteCpy = blackBishops;

    while (blackWhiteCpy != 0){
        uint64_t isolatedBishop = blackWhiteCpy & ((~blackWhiteCpy) + 1);
        uint8_t from = 63 - __builtin_ctzll(isolatedBishop);

        uint64_t attacks = getDiagonalAttacks(isolatedBishop, from);
        while (attacks != 0){
        uint64_t singleAttackSquare = attacks & ((~attacks) + 1);
        if (!blackPieceAtBitBoard(singleAttackSquare)){
            uint16_t move = movePack(from, 63-__builtin_ctzll(singleAttackSquare), false, false, 0, whitePieceAtBitBoard(singleAttackSquare), 0);
            // std::bitset<16> x(move);
            // std::cout << x << '\n';
            legal.push_back(move);
        }
        attacks &= ~singleAttackSquare; //37, 46
        }
        blackWhiteCpy &= ~isolatedBishop;
    }
  
    // printf("%d\n", legal.size());
}