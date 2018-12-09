#include "board.h"
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares

Board::Board(const uint32_t &Permutations, const uint8_t &N, uint8_t *Queens, bool *Early_Exit) : 
                permutations(Permutations), n(N), queens(Queens), early_exit(Early_Exit){
}


Board::~Board(){
}


int Board::validBoardCount(){
    int count = 0;
    while (!(*early_exit) && permutations-- > 0){
        if (validDiagonal()){
            ++count;
        }

        std::next_permutation(queens, queens + n);
    }
    return count;
}


bool Board::validDiagonal(){
    // Attempt 4
    for (int8_t i = 0; i < n; ++i){
        for (int8_t j = i + 1; j < n; ++j){
            if ((queens[i] + (j - i) == queens[j]) || (queens[i] - (j - i) == queens[j])){
                return false;
            }
        }
    }
    std::cout << "Arrangement: {";
    for (uint8_t k = 0; k < n - 1; ++k){
        std::cout << int(queens[k]) << ", ";
    }
    std::cout << int(queens[n-1]) << "} PASSED" << std::endl;
    return true;
}