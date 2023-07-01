#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>


std::vector<uint16_t> Board::getLegalWhiteQueenMoves(){
    std::vector<uint16_t> legal;

    uint64_t whiteQueensCpy = whiteQueens;

    uint64_t isolatedQueen = whiteQueensCpy & ((~whiteQueensCpy) + 1);

    while (whiteQueensCpy != 0){
        
        uint8_t from = 63 - __builtin_ctzll(isolatedQueen);

        uint64_t attacks = getFileAttacks(isolatedQueen, from) | getRankAttacks(isolatedQueen) | getDiagonalAttacks(isolatedQueen, from);
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
        whiteQueensCpy &= ~isolatedQueen;
    }
    // printf("%d\n", legal.size());
    return legal;
}

std::vector<uint16_t> Board::getLegalBlackQueenMoves(){
    std::vector<uint16_t> legal;

    uint64_t blackQueensCpy = blackQueens;

    uint64_t isolatedQueen = blackQueensCpy & ((~blackQueensCpy) + 1);

    while (blackQueensCpy != 0){
        
        uint8_t from = 63 - __builtin_ctzll(isolatedQueen);

        uint64_t attacks = getFileAttacks(isolatedQueen, from) | getRankAttacks(isolatedQueen) | getDiagonalAttacks(isolatedQueen, from);
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
        blackQueensCpy &= ~isolatedQueen;
    }
    // printf("%d\n", legal.size());
    return legal;
}