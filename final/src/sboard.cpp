#include "sboard.h"
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares

SBoard::SBoard(const uint64_t &Permutations, const uint8_t &N, const bool &PrintOut, uint8_t *Queens) : 
                permutations(Permutations), n(N), print_out(PrintOut), queens(Queens){
}


SBoard::~SBoard(){
}


void SBoard::validSBoardPermutations(){
    uint64_t temp = 0;
    uint64_t count = 0;
    while (!correctCount(n, count) && ++temp != permutations){
        if (validDiagonal()){
            ++count;
            if (print_out){
                printOut();
            }
        }
        std::next_permutation(queens, queens + n);
    }
    std::cout << "Found all valid queen positions: " << count << std::endl;
}


bool SBoard::validDiagonal(){
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

void SBoard::printOut(){
    std::cout << "Arrangement: {";
    for (uint8_t i = 0; i < n - 1; ++i){
        std::cout << int(queens[i]) << ", ";
    }
    std::cout << int(queens[n-1]) << "} PASSED" << std::endl;
}