#ifndef __SBOARD_H__
#define __SBOARD_H__

#include <stdint.h>     // better integers
#include <algorithm>    // next_permutation
#include <iostream>     // cout
#include "completion.h" // correctCount

class SBoard
{
public:
    SBoard(const uint64_t &Permutations,
           const uint8_t &N,
           const bool &PrintOut,
           uint8_t *Queens);            // Constructor
    ~SBoard();                          // Destructor
    
    void validSBoardPermutations();     // Runs permutations
    void printOut();                    // Prints valid board layouts


private:
    const uint64_t &permutations;       // Number of permutations
    const uint8_t &n;                   // "N" of n-queens
    const bool print_out;               // Whether or not to print
    uint8_t *queens;                    // Queen layout array
    
    bool validDiagonal();               // Checks for valid diagonals
};

#endif