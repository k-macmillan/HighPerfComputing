#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>     // better integers
// #include <iostream>     // cout
#include <algorithm>    // std::min


class Board
{
public:
    Board(const uint32_t &N, const uint32_t *Queens);
    ~Board();
    
    bool validBoard();
    bool validRow();

private:
    
    bool validDiagonal();

    const uint32_t n;
    const uint32_t *queens;
    uint32_t *valid_row = nullptr;
};

#endif