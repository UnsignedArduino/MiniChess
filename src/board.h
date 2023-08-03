#pragma once

#include <stdint.h>
#include <vector>

#define bitRead(value, bit) (((value) >> (63 - bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (63 - bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (63 - bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, 63 - bit) : bitClear(value, 63 - bit))

// 00 00 000000 000000
// 2 bits unused
// 2 bits r for promo
// 6 r to index
// 6 r from index
#define moveReadFromIndex(move) (move & 0b111111)
#define moveReadToIndex(move) (move & 0b111111000000)
#define moveReadPromotion(move) (move & 0b11000000000000)
void moveSet(uint16_t& move, uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture,
             uint8_t castle);
uint16_t movePack(uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture,
                  uint8_t castle);

class Board {
  public:
    Board();

    bool pieceAtIndex(uint8_t i);

    bool pieceAtBitBoard(uint64_t bb);
    bool whitePieceAtBitBoard(uint64_t bb);
    bool blackPieceAtBitBoard(uint64_t bb);
    bool isWhiteCheck();
    bool isBlackCheck();

    std::vector<uint16_t> legalWhiteMoves();
    std::vector<uint16_t> legalBlackMoves();

    uint64_t combinedOccupationBitBoard();
    uint64_t bitReverse(uint64_t bb);
    uint64_t flipVertical(uint64_t bb);
    uint64_t rotate90(uint64_t bb);
    uint64_t rotateAnti90(uint64_t bb);

    uint64_t getFileAttacks(uint64_t bitMask, uint16_t from);
    uint64_t getRankAttacks(uint64_t bitMask);
    uint64_t getDiagonalAttacks(uint64_t bitMask, uint16_t from);

    // Move Gen
    std::vector<uint16_t> getLegalWhitePawnMoves();
    std::vector<uint16_t> getLegalBlackPawnMoves();

    std::vector<uint16_t> getLegalWhiteRookMoves();
    std::vector<uint16_t> getLegalBlackRookMoves();

    std::vector<uint16_t> getLegalWhiteBishopMoves();
    std::vector<uint16_t> getLegalBlackBishopMoves();

    std::vector<uint16_t> getLegalWhiteQueenMoves();
    std::vector<uint16_t> getLegalBlackQueenMoves();

    std::vector<uint16_t> getLegalWhiteKingMoves();
    std::vector<uint16_t> getLegalBlackKingMoves();

    std::vector<uint16_t> getLegalWhiteKnightMoves();
    std::vector<uint16_t> getLegalBlackKnightMoves();

    // Perform Move
    uint8_t findPieceAtIndex(uint8_t from);
    void performMove(uint16_t move);
    void unMakeMove();

    void printBoard();
    void printBitBoard(uint64_t bb);

    uint64_t whitePawns;
    uint64_t blackPawns;
    uint64_t whiteKnights;
    uint64_t blackKnights;
    uint64_t whiteBishops;
    uint64_t blackBishops;
    uint64_t whiteRooks;
    uint64_t blackRooks;
    uint64_t whiteQueens;
    uint64_t blackQueens;
    uint64_t whiteKing;
    uint64_t blackKing;
    uint64_t** bitboards = new uint64_t*[12];
    uint8_t previousCapture = 0b00000000;
    uint8_t previousMovedPiece = 0b00000000;
    uint16_t previousMove = 0b0000000000000000;
    bool canWhiteQueenCastle = 1;
    bool canWhiteKingCastle = 1;
    bool canBlackQueenCastle = 1;
    bool canBlackKingCastle = 1;
    bool previousWhiteCastleKing = 0;
    bool previousWhiteCastleQueen = 0;
    bool previousBlackCastleKing = 0;
    bool previousBlackCastleQueen = 0;

    bool canPrevWhiteCastleKing = 0;
    bool canPrevWhiteCastleQueen = 0;
    bool canPrevBlackCastleKing = 0;
    bool canPrevBlackCastleQueen = 0;
};
