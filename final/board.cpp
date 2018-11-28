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
    return true;
}