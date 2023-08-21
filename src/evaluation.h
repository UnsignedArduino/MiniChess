#include "board.h"
#include <bitset>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>

// 0 white, 1, black
int16_t evaluate(Board& board);
uint64_t popcount64c(uint64_t x);

std::vector<int16_t> search();
int negaMax(Board &b, int alpha, int beta, uint8_t depth);
int alphaBeta(Board &b, int alpha, int beta, uint8_t depth);
