#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>     // better integers
#include <algorithm>    // next_permutation
#include <iostream>     // cout
#include <mpi.h>        // MPI_Send

// Class to run permutations and validation
class Board
{
public:
    Board(const uint64_t &Permutations,
          const uint8_t &N,
          const bool &PrintOut,
          uint8_t *Queens,
          bool *Early_Exit,
          uint32_t *Increment);         // Constructor
    ~Board();                           // Destructor
    
    void validBoardPermutations();      // Runs permutations
    void printOut();                    // Prints valid board layouts


private:
    const uint64_t &permutations;       // Number of permutations
    const uint8_t &n;                   // "N" of n-queens
    const bool print_out;               // Whether or not to print
    uint8_t *queens;                    // Queen layout array
    bool *early_exit;                   // Pointer to early_exit
    uint32_t *increment;                // Pointer to increment
    
    bool validDiagonal();               // Checks for valid diagonals
};

#endif