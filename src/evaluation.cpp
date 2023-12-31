#include "board.h"
#include <bitset>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "evaluation.h"
uint64_t popcount64c(uint64_t x)
{
    x -= (x >> 1) & 0x5555555555555555ULL;             //put count of each 2 bits into those 2 bits
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL); //put count of each 4 bits into those 4 bits 
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0fULL;        //put count of each 8 bits into those 8 bits 
    return (x * 0x0101010101010101ULL) >> 56;  //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ... 
}

int16_t evaluate(Board &board){
    // Attempt Chess 4.5 MS system
    // Material points, 10, 32, 35, 50, 90
    // pawn, knight, bishop, rook, queen
    
    uint8_t whitePawns = popcount64c(board.whitePawns);
    uint8_t blackPawns = popcount64c(board.blackPawns);
    uint16_t whiteMaterial = whitePawns*10 + 
                            popcount64c(board.whiteKnights)*32 + 
                            popcount64c(board.whiteBishops)*35 + 
                            popcount64c(board.whiteRooks)*50 + 
                            popcount64c(board.whiteQueens)*90;
    uint16_t blackMaterial = blackPawns*10 + 
                            popcount64c(board.blackKnights)*32 + 
                            popcount64c(board.blackBishops)*35 + 
                            popcount64c(board.blackRooks)*50 + 
                            popcount64c(board.blackQueens)*90;
    uint16_t MD = whiteMaterial > blackMaterial ? whiteMaterial - blackMaterial : blackMaterial-whiteMaterial;
    uint8_t PA = whiteMaterial > blackMaterial ? whitePawns : blackPawns;
    int16_t MS = std::min((uint16_t)240, MD) + (MD * PA * (800 - (blackMaterial + whiteMaterial))) / (640 * (PA + 1));
    MS = std::min((uint16_t)310, (uint16_t)MS);

    if (board.whosMove == 0){
        MS = whiteMaterial > blackMaterial ? MS : -MS;
    }
    if (board.whosMove == 1){
        MS = whiteMaterial > blackMaterial ? -MS : MS;
    }
    //MS = whiteMaterial > blackMaterial ? MS : -MS;
    // printf("MS: %d\n", MS);
    return MS;
}

// int negaScout(Board &b, int alpha, int beta, uint8_t depth){
//     if (depth == 0) return evaluate(b);
    
//     std::vector<uint16_t> moves = b.whosMove == 0 ? b.legalWhiteMoves() : b.legalBlackMoves();
//     int a, B, t, i;
//     B = beta;
//     a = alpha;
//     for (int i=0;i<moves.size();i++){
//         b.performMove(moves[i]);
//         // std::cout << "Turn: " << b.whosMove << " Move: "<< moves[i] <<"\n";
//         // b.printBoard();
//         t = -negaScout(b, -B, -alpha, depth-1);
//         if ((t > a) && (t < beta) && (i > 0)){
//             t = -negaScout(b, -beta, -alpha, depth - 1);
//         }
//         b.unMakeMove();
//         alpha = std::max( alpha, t );
//         if ( alpha >= beta )
//             return alpha;                            /* cut-off */
//         B = alpha + 1;        
//     }
//     return alpha;
// }
// int negaMax(Board &b, int alpha, int beta, uint8_t depth){
//     if (depth == 0)
//         return evaluate(b);
    
//     int value = -100000;
//     std::vector<uint16_t> moves = b.whosMove == 0 ? b.legalWhiteMoves() : b.legalBlackMoves();
//     for (int i=0;i<moves.size();i++){
//         b.performMove(moves[i]);
//         value = std::max(value, -negaMax(b, -beta, -alpha, depth - 1));
//         alpha = std::max(alpha, value);
//         if (alpha > beta){
//             break;
//         }
//         b.unMakeMove();
//     }
//     return value;
// }
int alphaBeta(Board &b, int alpha, int beta, uint8_t depth){
    if (depth == 0) return evaluate(b);
    std::vector<uint16_t> moves = b.whosMove == 0 ? b.legalWhiteMoves() : b.legalBlackMoves();
    for (int i=0;i<moves.size();i++){
        b.performMove(moves[i]);
        // std::cout << "Turn: " << b.whosMove << " Move: "<< moves[i] <<"\n";
        // b.printBoard();
        int score = -alphaBeta(b, -beta, -alpha, depth-1);
        b.unMakeMove();
        if( score >= beta )
         return beta;   //  fail hard beta-cutoff
        if( score > alpha )
            alpha = score; // alpha acts like max in MiniMax  
    }
    return alpha;
}