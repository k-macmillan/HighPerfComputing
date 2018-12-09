#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>     // better integers
#include <algorithm>    // min, max, next_permutation
#include <iostream>

class Board
{
public:
    Board(const uint32_t &Permutations, const uint8_t &N, uint8_t *Queens, bool *Early_Exit);
    ~Board();
    
    int validBoardCount();

private:
    uint32_t permutations;
    const uint8_t &n;
    uint8_t *queens;
    bool *early_exit;
    
    bool validDiagonal();
};

#endif