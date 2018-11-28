#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>     // better integers

class Board
{
public:
    Board(const uint32_t &N, const uint32_t *Queens, const uint32_t &Sum);
    ~Board();
    
    bool validBoard();

private:
    bool validRow();
    bool validDiagonal();

    const uint32_t n;
    const uint32_t *queens;
    const uint32_t row_sum;
};

#endif