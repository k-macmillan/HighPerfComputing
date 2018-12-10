#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>     // better integers
#include <algorithm>    // min, max, next_permutation
#include <iostream>

class Board
{
public:
    Board(const uint64_t &Permutations, const uint8_t &N, const bool &PrintOut, uint8_t *Queens, bool *Early_Exit);
    ~Board();
    
    int validBoardCount();
    void printOut();


private:
    const uint64_t &permutations;
    const uint8_t &n;
    const bool print_out;
    uint8_t *queens;
    bool *early_exit;
    
    bool validDiagonal();
    uint32_t test_count = 0;
};

#endif