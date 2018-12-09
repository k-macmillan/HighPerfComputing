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
    // validDiagonal();
    return count;
}


bool Board::validDiagonal(){
    // Attempt 3
    uint8_t max_col = 0;
    for (uint8_t i = 0; i < n; ++i){
        uint8_t col_dif = 1;
        // std::cout << "i: " << int(i);
        // This is only efficient at "high" n values
        max_col = std::min(std::max(queens[i], uint8_t(n - queens[i])), uint8_t(n - i)) + i;
        // std::cout << "\nMax columns: " << int(max_col) << std::endl;
        for (uint8_t j = i + 1; j < max_col; ++j){
            // std::cout << "\tj: " << int(j) << std::endl;
            if (queens[i] + col_dif == queens[j] || queens[i] - col_dif == queens[j]){
                // std::cout << "col diff: " << int(col_dif) << std::endl;
                // std::cout << "q[i]: " << int(queens[i]) << std::endl;
                // std::cout << "q[j]: " << int(queens[j]) << std::endl;
                // std::cout << "A: " << int(queens[i] + col_dif) << std::endl;
                // std::cout << "B: " << int(queens[i] - col_dif) << std::endl;

                // std::cout << "FAILURE AT: {" << int(i) << ", " << int(j) << "}\n" << std::endl;
                // std::cout << int(queens[n-1]) << "} FAILED" << std::endl;
                return false;
            }
            // https://web.archive.org/web/20120509225531/http://www2.research.att.com/~bs/JSF-AV-rules.pdf
            // AV Rule 199
            ++col_dif;
        }
        // std::cout << std::endl;
    }
    std::cout << "Arrangement: {";
    for (uint8_t k = 0; k < n - 1; ++k){
        std::cout << int(queens[k]) << ", ";
    }
    std::cout << int(queens[n-1]) << "} PASSED" << std::endl;
    return true;
}