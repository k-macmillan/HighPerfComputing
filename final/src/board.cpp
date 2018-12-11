#include "board.h"
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares



/*------------------------------------------------------------------------------
 * Function:    Constructor
 * Purpose:     Class to check a set number of permutations. Receives the 
 *              permutation to start on and sets variables. 
 *
 * In arg:      Permutations, N, PrintOut, Queens, Early_Exit, Increment
 */
Board::Board(const uint64_t &Permutations, const uint8_t &N, const bool &PrintOut, uint8_t *Queens, bool *Early_Exit, uint32_t *Increment) : 
                permutations(Permutations), n(N), print_out(PrintOut), queens(Queens), early_exit(Early_Exit){
    increment = Increment;
}



/*------------------------------------------------------------------------------
 * Function:    Deconstructor
 * Purpose:     Deconstructs the class.
 *
 */
Board::~Board(){
}



/*------------------------------------------------------------------------------
 * Function:    validBoardPermutations
 * Purpose:     Primary function call. This runs a lot and will check if a given 
 *              permutation is valid or not. If it is valid it will use MPI_Send 
 *              to relay that to the Master.
 *
 */
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
}



/*------------------------------------------------------------------------------
 * Function:    validDiagonal
 * Purpose:     Brute forces through all possible positions for a given layout 
 *              to ensure there are no conflicts.
 *
 * Returns:     bool
 */
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



/*------------------------------------------------------------------------------
 * Function:    printOut
 * Purpose:     Prints the current queens array. Only used for valid board 
 *              layouts.
 *
 */
void Board::printOut(){
    std::cout << "Arrangement: {";
    for (uint8_t i = 0; i < n - 1; ++i){
        std::cout << int(queens[i]) << ", ";
    }
    std::cout << int(queens[n-1]) << "} PASSED" << std::endl;
}
