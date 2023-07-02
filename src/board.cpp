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
    flags = 0b00000001;
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
  
}

uint16_t movePack(uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture, uint8_t castle) {
  uint16_t move = 0b0000000000000000;
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
  this->blackKing = 0x0800000000000000ULL;

  uint64_t** bitboards = new uint64_t*[12];
  this->bitboards[0] = &this->whitePawns;
  this->bitboards[1] = &this->whiteRooks;
  this->bitboards[2] = &this->whiteKnights;
  this->bitboards[3] = &this->whiteBishops;
  this->bitboards[4] = &this->whiteQueens;
  this->bitboards[5] = &this->whiteKing;
  this->bitboards[6] = &this->blackPawns;
  this->bitboards[7] = &this->blackRooks;
  this->bitboards[8] = &this->blackKnights;
  this->bitboards[9] = &this->blackBishops;
  this->bitboards[10] = &this->blackQueens;
  this->bitboards[11] = &this->blackKing;

  // this->whiteQueens = 0x0ULL;
  // this->whiteKnights = 0x0ULL;
  // this->whiteBishops = 0x0ULL;
  // this->blackQueens = 0x0ULL;
  // this->blackKnights = 0x0ULL;
  // this->blackBishops = 0x0ULL;
  // this->blackKnights >>= 32;
  // this->blackKing >>= 24;
  // this->whiteQueens <<= 24;
  // this->whiteBishops = 0x0000000000000024ULL << 24;
  // this->whiteRooks = 0x0000000081000000ULL << 3;
  // this->whitePawns = 0x000000010100FF00ULL;
  // this->blackPawns = 0x0000000000FF0000;
  // this->blackPawns = 0;
  // this->blackRooks = 0;

  //this->blackPawns = 0x0ULL;
  // this->whiteKnights <<= 48;
  //this->whitePawns = 0x0ULL;
  //this->blackBishops >>= 40;
  //this->blackKnights >>= 48;
  // this->getLegalWhiteBishopMoves();
  // this->getLegalBlackQueenMoves();
  // this->getLegalBlackKingMoves();
  // this->getLegalBlackKnightMoves();
  // printf("%d\n", this->isBlackCheck());
}

bool Board::isWhiteCheck(){
  uint64_t explosion = 0x0ULL;
  uint16_t from = 63 - __builtin_ctzll(whiteKing);
  // Rook/Queen posibilities
  explosion |= getFileAttacks(whiteKing, from); 
  explosion |= getRankAttacks(whiteKing);
  while (explosion != 0){
    uint64_t isolated = explosion & ((~explosion) + 1);
    if ((blackQueens & isolated) != 0 || (blackRooks & isolated) != 0){
      return true;
    }
    explosion &= ~isolated;
  }
  explosion = getDiagonalAttacks(whiteKing, from);
  while (explosion != 0){
    uint64_t isolated = explosion & ((~explosion) + 1);

    if ((blackQueens & isolated) != 0 || (blackBishops & isolated) != 0){
      return true;
    }
    explosion &= ~isolated;
  }
  explosion = 0x0ULL;
  if (from % 8 != 0){
      explosion |= whiteKing << 17;
      explosion |= whiteKing >> 15;
      // Not A or B file
      if ((from - 1) % 8 != 0){
          explosion |= whiteKing << 10;
          explosion |= whiteKing >> 6;
      }
  }

  // Not H File
  if ((from+1) % 8 != 0){
      explosion |= whiteKing << 15;
      explosion |= whiteKing >> 17;
      // Not H or F File
      if ((from+2) % 8 != 0){
          explosion |= whiteKing << 6;
          explosion |= whiteKing >> 10;
      }
  }
  while (explosion != 0){
    uint64_t isolated = explosion & ((~explosion) + 1);
    if ((blackKnights & isolated) != 0){
      return true;
    }
    explosion &= ~isolated;
  }
  return false;
  
}

bool Board::isBlackCheck(){
  uint64_t explosion = 0x0ULL;
  uint16_t from = 63 - __builtin_ctzll(blackKing);
  // Rook/Queen posibilities
  explosion |= getFileAttacks(blackKing, from); 
  explosion |= getRankAttacks(blackKing);
  while (explosion != 0){
    uint64_t isolated = explosion & ((~explosion) + 1);
    if ((whiteQueens & isolated) != 0 || (whiteRooks & isolated) != 0){
      return true;
    }
    explosion &= ~isolated;
  }
  explosion = getDiagonalAttacks(blackKing, from);
  while (explosion != 0){
    uint64_t isolated = explosion & ((~explosion) + 1);

    if ((whiteQueens & isolated) != 0 || (whiteBishops & isolated) != 0){
      return true;
    }
    explosion &= ~isolated;
  }
  explosion = 0x0ULL;
  if (from % 8 != 0){
      explosion |= blackKing << 17;
      explosion |= blackKing >> 15;
      // Not A or B file
      if ((from - 1) % 8 != 0){
          explosion |= blackKing << 10;
          explosion |= blackKing >> 6;
      }
  }

  // Not H File
  if ((from+1) % 8 != 0){
      explosion |= blackKing << 15;
      explosion |= blackKing >> 17;
      // Not H or F File
      if ((from+2) % 8 != 0){
          explosion |= blackKing << 6;
          explosion |= blackKing >> 10;
      }
  }
  while (explosion != 0){
    uint64_t isolated = explosion & ((~explosion) + 1);
    if ((whiteKnights & isolated) != 0){
      return true;
    }
    explosion &= ~isolated;
  }
  return false;
  
}

std::vector<uint16_t> Board::legalWhiteMoves(){
  std::vector<uint16_t> legal;
  std::vector<uint16_t> temp = getLegalWhitePawnMoves();
  // Test pawn moves
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isWhiteCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalWhiteKnightMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isWhiteCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalWhiteBishopMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isWhiteCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalWhiteRookMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isWhiteCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalWhiteKingMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isWhiteCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalWhiteQueenMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isWhiteCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  // Check for Castle
  // printf("%d\n", canWhiteKingCastle);
  if (canWhiteKingCastle){
    // No pieces between
    if ((combinedOccupationBitBoard() & 0x000000000000006ULL) == 0){
      whiteKing = 0x000000000000004ULL;
      if (!isWhiteCheck()){
        whiteKing = 0x000000000000002ULL;
        if (!isWhiteCheck()){
          legal.push_back(0b0010000001111100);
          // printf("Castle\n");
        }
      }
      whiteKing = 0x000000000000008ULL;
    }
  }
  if (canWhiteQueenCastle){
    // No pieces between
    if ((combinedOccupationBitBoard() & 0x000000000000070ULL) == 0){
      whiteKing = 0x000000000000008ULL << 1; // Math hard
      if (!isWhiteCheck()){
        whiteKing <<= 1;
        if (!isWhiteCheck()){
          whiteKing <<= 1;
          if (!isWhiteCheck()){
            legal.push_back(0b0011000001111100);
          }
          // printf("Castle\n");
        }
      }
      whiteKing = 0x000000000000008ULL;
    }
  }
  return legal;
}


std::vector<uint16_t> Board::legalBlackMoves(){
  std::vector<uint16_t> legal;
  std::vector<uint16_t> temp = getLegalBlackPawnMoves();
  // Test pawn moves
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isBlackCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalBlackKnightMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isBlackCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalBlackBishopMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isBlackCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalBlackRookMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isBlackCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalBlackKingMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isBlackCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  temp.clear();
  temp = getLegalBlackQueenMoves();
  for (int i = 0; i < temp.size(); i++){
    performMove(temp[i]);
    if (!isBlackCheck()){
      legal.push_back(temp[i]);
    }
    unMakeMove();
  }
  // Check for Castle
  // printf("%d\n", canWhiteKingCastle);
  if (canBlackKingCastle){
    // No pieces between
    if ((combinedOccupationBitBoard() & (0x000000000000006ULL << 56)) == 0){
      blackKing = 0x000000000000004ULL << 56;
      if (!isBlackCheck()){
        blackKing = 0x000000000000002ULL << 56;
        if (!isBlackCheck()){
          legal.push_back(0b0010000000000100);
          // printf("Castle\n");
        }
      }
      blackKing = 0x000000000000008ULL << 56;
    }
  }
  if (canBlackQueenCastle){
    // No pieces between
    if ((combinedOccupationBitBoard() & (0x000000000000070ULL << 56)) == 0){
      blackKing = 0x000000000000008ULL << 1 << 56; // Math hard
      if (!isWhiteCheck()){
        blackKing <<= 1;
        if (!isWhiteCheck()){
          blackKing <<= 1;
          if (!isWhiteCheck()){
            legal.push_back(0b0011000000000100);
          }
          // printf("Castle\n");
        }
      }
      blackKing = 0x000000000000008ULL << 56;
    }
  }
  return legal;
}




uint8_t Board::findPieceAtIndex(uint8_t from){
  if (bitRead(whitePawns, from) == 1){
    return 1;
  }
  else if (bitRead(whiteRooks, from) == 1){
    return 2;
  }
  else if (bitRead(whiteKnights, from) == 1){
    return 3;
  }
  else if (bitRead(whiteBishops, from) == 1){
    return 4;
  }
  else if (bitRead(whiteQueens, from) == 1){
    return 5;
  }
  else if (bitRead(whiteKing, from) == 1){
    return 6;
  }
  else if (bitRead(blackPawns, from) == 1){
    return 7;
  }
  else if (bitRead(blackRooks, from) == 1){
    return 8;
  }
  else if (bitRead(blackKnights, from) == 1){
    return 9;
  }
  else if (bitRead(blackBishops, from) == 1){
    return 10;
  }
  else if (bitRead(blackQueens, from) == 1){
    return 11;
  }
  else if (bitRead(blackKing, from) == 1){
    return 12;
  }
  return 0;
}

void Board::performMove(uint16_t move){
  uint8_t from = move & 0b0000000000111111;
  uint8_t to = (move & 0b0000111111000000) >> 6;
  uint8_t flag = (move & 0b1111000000000000) >> 12;
  uint64_t fromMask = 0x01ULL << (63-from);
  uint64_t toMask = 0x01ULL << (63-to);
  uint8_t fromPiece = findPieceAtIndex(from);
  previousMovedPiece = fromPiece;
  // Move piece from its bitboard to its new position
  
  //If there was a capture, remove the captured piece
  if (flag == 0b0100 || flag >= 0b1100){
    uint8_t toBeCaptured = findPieceAtIndex(to);
    // printf("Capturing: %d\n", to);
    *bitboards[toBeCaptured-1] = (*bitboards[toBeCaptured-1] & (~toMask));
    previousCapture = toBeCaptured;
  }
  
  // knight Promo
  // std::bitset<16> x(move);
  // std::cout << "Flag:" << x << "\n";
  // printf("%d\n", fromPiece);
  if (flag == 0b1000 || flag == 0b1100){
    // Remove piece
    *bitboards[fromPiece-1] &= ~fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 2 : 8] |= toMask;
  }
  else if (flag == 0b1001 || flag == 0b1101){
    // Remove piece
    *bitboards[fromPiece-1] &= ~fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 3 : 9] |= toMask;
  }
  else if (flag == 0b1010 || flag == 0b1110){
    // Remove piece
    *bitboards[fromPiece-1] &= ~fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 1 : 7] |= toMask;
  }
  else if (flag == 0b1011 || flag == 0b1111){
    // Remove piece
    *bitboards[fromPiece-1] &= ~fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 4 : 10] |= toMask;
  }
  else  if (flag != 0b0010 && flag != 0b0011){
    // Move Origin piece to its new spot
    *bitboards[fromPiece-1] = (*bitboards[fromPiece-1] & (~fromMask)) | toMask;
  }

  if (flag == 0b0010){
    if (fromPiece <= 6){
      whiteKing = 0x000000000000002ULL;
      whiteRooks &= ~0x000000000000001ULL;
      whiteRooks |= 0x000000000000004ULL;
      previousWhiteCastleKing = 1;
      canPrevWhiteCastleKing = 1;
    }
    else{
      blackKing = 0x0800000000000000ULL >> 2;
      blackRooks &= ~(0x0800000000000000ULL >> 3);
      blackRooks |= 0x0800000000000000ULL >> 1;
      previousBlackCastleKing = 1;
      canPrevBlackCastleKing = 1;
    }
  }
  if (flag == 0b0011){
    if (fromPiece <= 6){
      whiteKing = 0x000000000000008ULL << 2;
      whiteRooks &= ~0x000000000000080ULL;
      whiteRooks |= 0x000000000000008ULL << 1;
      previousWhiteCastleQueen = 1;
      canPrevWhiteCastleQueen = 1;
    }
    else{
      blackKing = 0x0800000000000000ULL << 2;
      blackRooks &= ~0x8000000000000000ULL;
      blackRooks |= 0x0800000000000000ULL << 1;
      previousBlackCastleQueen = 1;
      canPrevBlackCastleQueen = 1;
    }
    
  }
  previousMove = move;

  // Figure out castling flags
  if (bitRead(whiteRooks, 63) != 1 || whiteKing != 0x000000000000008ULL){
    canWhiteKingCastle = 0;
    canPrevWhiteCastleKing = 1;
    
  }
  if (bitRead(whiteRooks, 56) != 1 || whiteKing != 0x000000000000008ULL){
    canWhiteQueenCastle = 0;
    canPrevWhiteCastleQueen = 1;
  }
  if (bitRead(blackRooks, 7) != 1 || blackKing != 0x0800000000000000ULL){
    canBlackKingCastle = 0;
    canPrevBlackCastleKing = 1;
  }
  if (bitRead(blackRooks, 0) != 1 || blackKing != 0x0800000000000000ULL){
    canBlackQueenCastle = 0;
    canPrevBlackCastleQueen = 1;
    
  }

  // printBitBoard(whitePawns);
  // printBitBoard(blackPawns);
}

void Board::unMakeMove(){
  //printBitBoard(blackRooks);
  uint8_t from = previousMove & 0b0000000000111111;
  uint8_t to = (previousMove & 0b0000111111000000) >> 6;
  uint8_t flag = (previousMove & 0b1111000000000000) >> 12;
  uint64_t fromMask = 0x01ULL << (63-from);
  uint64_t toMask = 0x01ULL << (63-to);
  uint8_t fromPiece = previousMovedPiece;

  //If there was a capture, remove the captured piece
  if (flag == 0b0100 || flag >= 0b1100){
    uint8_t toBeCaptured = previousCapture;
    // printf("Capturing: %d\n", to);
    *bitboards[toBeCaptured-1] = (*bitboards[toBeCaptured-1] | toMask);
  }
  
  // knight Promo
  // std::bitset<8> x(flag);
  // std::cout << "Flag:" << x << "\n";
  if (flag == 0b1000 || flag == 0b1100){
    // Remove piece
    *bitboards[fromPiece-1] |= fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 2 : 8] &= ~toMask;
  }
  else if (flag == 0b1001 || flag == 0b1101){
    // Remove piece
    *bitboards[fromPiece-1] |= fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 3 : 9] &= ~toMask;
  }
  else if (flag == 0b1010 || flag == 0b1110){
    // Remove piece
    *bitboards[fromPiece-1] |= fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 1 : 7] &= ~toMask;
  }
  else if (flag == 0b1011 || flag == 0b1111){
    // Remove piece
    *bitboards[fromPiece-1] |= fromMask;
    // Replace with promotion
    *bitboards[fromPiece <= 6 ? 4 : 10] &= ~toMask;
  }
  else if (!previousWhiteCastleKing && !previousWhiteCastleQueen && !previousBlackCastleKing && !previousBlackCastleQueen){
    // Move Origin piece to its new spot
    
    *bitboards[fromPiece-1] = (*bitboards[fromPiece-1] & (~toMask)) | fromMask;
  }


  // Figure out castling flags
  // printf("Castle %d %d %d %d\n", previousWhiteCastleKing, previousWhiteCastleQueen, previousBlackCastleKing, previousBlackCastleQueen);
  if (canPrevWhiteCastleKing){
    canWhiteKingCastle = 1;
  }
  if (canPrevWhiteCastleQueen){
    canWhiteQueenCastle = 1;
  }
  if (canPrevBlackCastleKing){
    canBlackKingCastle = 1;
  }
  if (canPrevBlackCastleQueen){
    canBlackQueenCastle = 1;
  }

  if (previousWhiteCastleKing){
    canWhiteKingCastle = 1;
    whiteKing = 0x000000000000008ULL;
    whiteRooks &= ~0x000000000000004ULL;
    whiteRooks |= 0x000000000000001ULL;
  }
  if (previousWhiteCastleQueen){
    canWhiteQueenCastle = 1;
    whiteKing = 0x000000000000008ULL;
    whiteRooks &= ~(0x000000000000008ULL << 1);
    whiteRooks |= 0x000000000000080ULL;
  }
  if (previousBlackCastleKing){
    canBlackKingCastle = 1;
    
    blackKing = 0x0800000000000000ULL;
    blackRooks &= ~(0x0800000000000000ULL >> 1);
    blackRooks |= 0x0800000000000000ULL >> 3;
    
  }
  if (previousBlackCastleQueen){
    canBlackQueenCastle = 1;
    blackKing = 0x0800000000000000ULL;
    blackRooks &= ~(0x0800000000000000ULL << 1);
    blackRooks |= 0x0800000000000000ULL << 4;
  }
  previousWhiteCastleKing = 0;
  previousWhiteCastleQueen = 0;
  previousBlackCastleKing = 0;
  previousBlackCastleQueen = 0;
  canPrevWhiteCastleKing = 0;
  canPrevWhiteCastleQueen = 0;
  canPrevBlackCastleKing = 0;
  canPrevBlackCastleQueen = 0;
  previousCapture = 0b00000000;
  previousMovedPiece = 0b00000000;
  previousMove = 0b0000000000000000;

  //printBitBoard(blackRooks);
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
