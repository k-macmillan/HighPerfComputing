/* File:        Prog2.cu
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
#include <stdlib.h>     // srand, rand
#include <mpi.h>        // mpi
#include <string>       // Strings

#include "completion.h" // Completeness function/array
#include "board.h"
#include "nthpermutation.h"

#ifdef DEBUG
void singleTest(){
    uint32_t n = 5;
    uint32_t queens[6] = {0, 1, 2, 3, 4, 5};

    Board test = Board(n, queens);
    bool temp = test.validBoard();
    if (temp){
        // std::cout << "TRUE" << std::endl;
        std::cout << "\n{";
        for (uint32_t i = 0; i < n; ++i){
            std::cout << queens[i] << ", ";
        }
        std::cout << queens[n] << "}\n" << std::endl;        
    }
    else{
        std::cout << "\n{";
        for (uint32_t i = 1; i < n; ++i){
            std::cout << queens[i] << ", ";
        }
        std::cout << queens[n] << "} is NOT valid.\n" << std::endl; 
    }
}
#endif

int main (int argc, char** argv){
    // Can you do argument error checking with MPI?
    bool user_defined = false;
    uint8_t n;
    int id;             // ID
    int p;              // Processor count
    MPI_Init(&argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    try{
        n = stoi(std::string(argv[1]));
        user_defined = true;
    }catch(std::exception const & e){
        if (id == 0)
            std::cout << "An error occured: " << e.what() << std::endl;
        MPI_Finalize();
        return 0;
    }



    if (n > 20){
        if (id == 0)
            std::cout << "\nYour n: " << argv[1] << ", is too large. Please pick a number under 21\n" << std::endl;
        MPI_Finalize();
        return 0;
    }

#ifdef DEBUG
    singleTest();
    return 0;
#endif

    if (user_defined){
        // Run "n" queens on the N passed in
        for(uint8_t i = 0; i < n; ++i){
            // Run each "n" queens
            continue;
        }
    }
    else{
        if (id == 0){
            for(uint8_t i = 0; i < 21; ++i){
                // Run each "n" queens
                continue;
            }
        }
        else{

        }
    }

    MPI_Finalize();
    return 0;
}

