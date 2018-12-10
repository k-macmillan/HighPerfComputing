#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>     // better integers
#include <algorithm>    // min, max, next_permutation
#include <iostream>
#include <mpi.h>        // mpi

class Board
{
public:
    Board(const uint64_t &Permutations, const uint8_t &N, const bool &PrintOut, uint8_t *Queens, bool *Early_Exit, uint32_t *Increment);
    ~Board();
    
    void validBoardPermutations();
    void printOut();


private:
    const uint64_t &permutations;
    const uint8_t &n;
    const bool print_out;
    uint8_t *queens;
    bool *early_exit;
    uint32_t *increment;
    
    bool validDiagonal();
};

#endif