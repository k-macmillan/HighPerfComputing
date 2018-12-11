/* File:        final.cpp
 *
 * Author:      Kyle MacMillan
 *
 * Assignment:  Final Project
 *
 * Course:      CSC 792 - High Performance Computing
 *
 * Purpose:     Completion of final project. Please see github repository for a 
 *              detailed writeup. 
 *              https://github.com/macattackftw/HighPerfComputing
 *
 * Compile:     
 *              make (with provided Makefile)
 *
 * Run:         ./final #
 *              ./final # printout
 *              
 *
 * Output:      None or valid queen layouts if printout
 *
 * Notes:       Works perfectly
 *
 */     

#include <iostream>     // cout
#include <stdint.h>     // better integers
#include <string>       // Strings
#include "sboard.h"
#include "completion.h" // Completeness function/array
#include "nthpermutation.h"
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares



/*------------------------------------------------------------------------------
 * Function:    main
 * Purpose:     Checks user input and runs permutations
 *
 * In arg:      argc, argv
 */
int main (int argc, char** argv){
    uint8_t n = 10;             // User defined n
    bool print_out = false;     // Print or not

    // Handle user input
    try{
        if (argc > 1){
            n = stoi(std::string(argv[1]));
            if (n > 19){
                std::cout << "\nYour n: " << argv[1] << ", is too large. " 
                          << "Please pick a number under 19. Aborting...\n" << std::endl;
                return 0;
            }
            if (argc > 2){
                if (std::string(argv[2]) == "printout"){
                    print_out = true;
                }
            }
        }
        else{
            std::cout << "Invalid command line arguments.\n" 
                      << "mpirun -np ## ./final #" << std::endl;
            return 0;
        }
    }catch(std::exception const & e){
        std::string no_n = "basic_string::_M_construct null not valid";
        if (std::string(e.what()) != no_n){
            std::cout << "An error occurred: " << e.what() 
                      << ", aborting..." << std::endl;
            return 0;
        }
    }

    // Allocate array and initialize
    uint8_t *queens = (uint8_t*)malloc(n * sizeof(uint8_t));
    for (uint8_t i = 0; i < n; ++i){
        queens[i] = i;
    }
    std::cout << "Running " << int(n) << "-queens..." << std::endl;

    // Instantiate and permute!
    SBoard b(factorials[n], n, print_out, queens);
    b.validSBoardPermutations();

    // Free memory
    free(queens);
   
    return 0;

}
