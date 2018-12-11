#include "board.h"
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares

Board::Board(const uint64_t &Permutations, const uint8_t &N, const bool &PrintOut, uint8_t *Queens, bool *Early_Exit, uint32_t *Increment) : 
                permutations(Permutations), n(N), print_out(PrintOut), queens(Queens), early_exit(Early_Exit){
    increment = Increment;
}


Board::~Board(){
}


void Board::validBoardPermutations(){
    uint64_t temp = 0;
    while (!(*early_exit) && ++temp != permutations){
        if (validDiagonal()){
            MPI_Send(increment, 1, MPI::UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD);
            if (print_out){
                printOut();
            }
        }
        std::next_permutation(queens, queens + n);
    }
    if (temp == permutations){
        *early_exit = true;
        MPI_Bcast(&early_exit, 1, MPI::BOOL, 0, MPI_COMM_WORLD);
    }
}


bool Board::validDiagonal(){
    // Attempt 4
    for (uint8_t i = 0; i < n; ++i){
        for (uint8_t j = i + 1; j < n; ++j){
            if (queens[i] + (j - i) == queens[j] || queens[i] - (j - i) == queens[j]){
                return false;
            }
        }
    }
    return true;
}

void Board::printOut(){
    std::cout << "Arrangement: {";
    for (uint8_t i = 0; i < n - 1; ++i){
        std::cout << int(queens[i]) << ", ";
    }
    std::cout << int(queens[n-1]) << "} PASSED" << std::endl;
}
