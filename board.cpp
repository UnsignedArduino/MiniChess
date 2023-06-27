#include "board.h"
#include <string.h>
#include <stdio.h>

Piece::Piece() {
  this->type = EMPTY;
}

Piece::Piece(char type) {
  this->type = type;
}

Board::Board() {
  this->board[0][0] = Piece(BLACK_ROOK);
  this->board[0][1] = Piece(BLACK_KNIGHT);
  this->board[0][2] = Piece(BLACK_BISHOP);
  this->board[0][3] = Piece(BLACK_QUEEN);
  this->board[0][4] = Piece(BLACK_KING);
  this->board[0][5] = Piece(BLACK_BISHOP);
  this->board[0][6] = Piece(BLACK_KNIGHT);
  this->board[0][7] = Piece(BLACK_ROOK);
  for (uint8_t x = 0; x < 8; x ++) {
    this->board[1][x] = Piece(BLACK_PAWN);
  }
  for (uint8_t y = 2; y < 6; y ++) {
    for (uint8_t x = 0; x < 8; x ++) {
      this->board[y][x] = Piece(EMPTY);
    }
  }
  for (uint8_t x = 0; x < 8; x ++) {
    this->board[6][x] = Piece(WHITE_PAWN);
  }
  this->board[7][0] = Piece(WHITE_ROOK);
  this->board[7][1] = Piece(WHITE_KNIGHT);
  this->board[7][2] = Piece(WHITE_BISHOP);
  this->board[7][3] = Piece(WHITE_QUEEN);
  this->board[7][4] = Piece(WHITE_KING);
  this->board[7][5] = Piece(WHITE_BISHOP);
  this->board[7][6] = Piece(WHITE_KNIGHT);
  this->board[7][7] = Piece(WHITE_ROOK);
}

void Board::printBoard(bool pretty/* = false*/) {
  for (uint8_t y = 0; y < 8; y ++) {
    for (uint8_t x = 0; x < 8; x ++) {
      if (pretty) {
        this->printPrettyPiece(this->board[y][x].type);
      } else {
        printf("%c", this->board[y][x].type);
      }
      printf(" ");
    }
    printf("\n");
  }
}

void Board::printPrettyPiece(char algebraic) {
  switch (algebraic) {
    case (WHITE_PAWN): {
      printf("♙");
      break;
    }
    case (BLACK_PAWN): {
      printf("♟︎");
      break;
    }
    case (WHITE_KNIGHT): {
      printf("♘");
      break;
    }
    case (BLACK_KNIGHT): {
      printf("♞");
      break;
    }
    case (WHITE_BISHOP): {
      printf("♗");
      break;
    }
    case (BLACK_BISHOP): {
      printf("♝");
      break;
    }
    case (WHITE_ROOK): {
      printf("♖");
      break;
    }
    case (BLACK_ROOK): {
      printf("♜");
      break;
    }
    case (WHITE_QUEEN): {
      printf("♕");
      break;
    }
    case (BLACK_QUEEN): {
      printf("♛");
      break;
    }
    case (WHITE_KING): {
      printf("♔");
      break;
    }
    case (BLACK_KING): {
      printf("♚");
      break;
    }
    default:
    case (EMPTY): {
      printf(" ");
      break;
    }
  }
}
