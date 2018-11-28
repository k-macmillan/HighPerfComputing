#include "board.h"

Board::Board(const uint32_t &N, const uint32_t * const Queens, const uint32_t &Sum) : n(N),
                                                                                      queens(Queens),
                                                                                      row_sum(Sum){
}


Board::~Board(){
    queens = NULL;
}


bool Board::validBoard(){
    return validRow() && validDiagonal()
}


bool Board::validRow(){
    uint32_t sum = 0;
    for (uint32_t i = 0; i < n; ++i){
        sum += queens[i];
    }
    return sum == row_sum;
}


bool Board::validDiagonal(){
    /*  For each element in queens if 
     *  j_val - i_val + j == i_val or zero we are on a diagonal.
     *  
     */
    for (uint32_t i = 0; i < n; ++i){
        for (uint32_t j = i; j < n; ++j){
            diff = queens[j] - queens[i] + j - i;
            if (diff == 0 || diff == queens[i]){
                return false;
            }
        }
    }
    return true;
}