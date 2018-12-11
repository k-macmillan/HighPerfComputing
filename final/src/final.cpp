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

#include <iostream>         // cout
#include <stdint.h>         // better integers
#include <string>           // Strings
#include <mpi.h>            // mpi

#include "board.h"          // Handles k permutations
#include "completion.h"     // Completeness function/array
#include "nthpermutation.h" // Calculates the nth permutation
#pragma GCC diagnostic ignored "-Wsign-compare"   // Ignore sign compares


int main (int argc, char** argv){
    uint8_t n = 10;             // User defined n
    int id;                     // ID
    int p;                      // Processor count
    bool early_exit = false;    // Early exit for grads
    bool print_out = false;
    uint32_t global_count = 0;

    MPI_Init(&argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    p -= 1;

    if (p == 0){
        std::cout << "Invalid usage:\nProcessors must be > 1" << std::endl;
        MPI_Finalize();
        return 0;
    }

    // Handle user input
    try{
        if (argc > 1){
            n = stoi(std::string(argv[1]));
            if (n > 19){
                if (id == 0)
                    std::cout << "\nYour n: " << argv[1] << ", is too large. " << 
                    "Please pick a number under 19. Aborting...\n" << std::endl;
                MPI_Finalize();
                return 0;
            }
            if (argc > 2){
                if (std::string(argv[2]) == "printout"){
                    print_out = true;
                }
            }
        }
        else{
            if (id == 0){
                std::cout << "Invalid command line arguments.\n" 
                          << "mpirun -np ## ./final #" << std::endl;
            }
            MPI_Finalize();
            return 0;
        }
    }catch(std::exception const & e){
        if (id == 0){
            std::string no_n = "basic_string::_M_construct null not valid";
            if (std::string(e.what()) != no_n){
                std::cout << "An error occurred: " << e.what() 
                          << ", aborting..." << std::endl;
                MPI_Finalize();
                return 0;
            }
        }
    }




    // Run "n" queens on the N passed in
    uint32_t local = 1;
    if (id == 0){
        std::cout << "Running " << int(n) << "-queens..." << std::endl;
        while (!correctCount(n, global_count) && !early_exit){
            MPI_Recv(&local, 1, MPI::UNSIGNED_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_count += local;
        }
        std::cout << "Found all valid queen positions: " << global_count << std::endl;
        early_exit = true;
        MPI_Bcast(&early_exit, 1, MPI::BOOL, 0, MPI_COMM_WORLD);
    }
    else{
        uint8_t *queens = (uint8_t*)malloc(n * sizeof(uint8_t));
        for (uint8_t i = 0; i < n; ++i){
            queens[i] = i;
        }
        uint64_t rank_perms = factorials[n] / p;
        if (id != 1){
            // Skip this for id == 1
            uint64_t id_rank_perms = rank_perms * (id - 1) + factorials[n] % p;
            nthPermutation(n, id_rank_perms, queens);
        }
        else{
            rank_perms = (factorials[n] / p) + (factorials[n] % p);
        }
        

        Board b(rank_perms, n, print_out, queens, &early_exit, &local);
        b.validBoardPermutations();
        free(queens);
    }


    MPI_Finalize();
    return 0;
}
