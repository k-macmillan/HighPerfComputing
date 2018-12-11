#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>     // better integers
#include <algorithm>    // next_permutation
#include <iostream>     // cout
#include <mpi.h>        // MPI_Send

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
    uint32_t end = 0;

    bool validDiagonal();
};

#endif