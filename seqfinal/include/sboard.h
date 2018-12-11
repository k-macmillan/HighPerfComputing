#ifndef __SBOARD_H__
#define __SBOARD_H__

#include <stdint.h>     // better integers
#include <algorithm>    // next_permutation
#include <iostream>     // cout
#include "completion.h" // correctCount

class SBoard
{
public:
    SBoard(const uint64_t &Permutations, const uint8_t &N, const bool &PrintOut, uint8_t *Queens);
    ~SBoard();
    
    void validSBoardPermutations();
    void printOut();


private:
    const uint64_t &permutations;
    const uint8_t &n;
    const bool print_out;
    uint8_t *queens;
    
    bool validDiagonal();
};

#endif