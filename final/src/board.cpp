#include "board.h"
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares

Board::Board(const uint32_t &N, const uint32_t *Queens) : n(N),
                                                          queens(Queens){
    valid_row = new uint32_t[N + 1]();
}


Board::~Board(){
    delete [] valid_row;
}


bool Board::validBoard(){
    return validRow() && validDiagonal();
}


bool Board::validRow(){
    uint32_t sum = 0;
    for (uint32_t i = 0; i <= n; ++i){
        valid_row[queens[i]] = 1;
    }
    for (uint32_t i = 0; i <= n; ++i){
        sum += valid_row[i];
    }
    return sum == n + 1? true : false;
}


bool Board::validDiagonal(){
    // Smart checking
    for (uint32_t i = 1; i < n; ++i){
        // Search "up"
        for (uint32_t j = i + 1; j < std::min(queens[i], n - i + 1) + i; ++j){
            if (queens[i] - (j - i) == queens[j]){
                return false;
            }
        }
        // Search "down"
        for (uint32_t j = i + 1; j < std::min(n - queens[i] + 1, n - i + 1) + i; ++j){
            if (queens[i] + (j - i) == queens[j]){
                return false;
            }
        }
    }
    return true;

    // Brute force
    // for (uint32_t i = 1; i < n; ++i){
    //     for (uint32_t j = i + 1; j <= n; ++j){
    //         uint32_t col_dif = j - i;
    //         if (queens[i] - col_dif > 0){
    //             if (queens[j] == queens[i] - col_dif || queens[j] == queens[i] + col_dif){
    //                 return false;
    //             }
    //         }

    //         if (queens[i] + col_dif <= n){
    //             if (queens[j] == queens[i] - col_dif || queens[j] == queens[i] + col_dif){
    //                 return false;
    //             }
    //         }
    //     }
    // }
    // return true;
}