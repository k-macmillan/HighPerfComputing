/* File:        final.cpp
 *
 * Author:      Kyle MacMillan
 *
 * Assignment:  Final Project
 *
 * Course:      CSC 792 - High Performance Computing
 *
 * Purpose:     
 *
 * Compile:     
 *              make (with provided Makefile)
 *
 * Run:         
 *              
 *
 * Output:      
 *
 * Notes:       None, runs like a dream
 *
 */        

#include <iostream>     // cout
#include <stdint.h>     // better integers
#include <string>       // Strings
#include "sboard.h"
#include "completion.h" // Completeness function/array
#include "nthpermutation.h"
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares


int main (int argc, char** argv){
    uint8_t n = 10;             // User defined n
    bool print_out = false;

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

    uint8_t *queens = (uint8_t*)malloc(n * sizeof(uint8_t));
    for (uint8_t i = 0; i < n; ++i){
        queens[i] = i;
    }
    std::cout << "Running " << int(n) << "-queens..." << std::endl;
    SBoard b(factorials[n], n, print_out, queens);
    b.validSBoardPermutations();
    free(queens);
   
    return 0;

}
