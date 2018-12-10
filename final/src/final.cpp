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
#include <queue>        // Queue

#include "completion.h" // Completeness function/array
#include "board.h"
#include "nthpermutation.h"
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
    bool user_defined = false;  // Flag for user-defined n
    uint8_t n;                  // User defined n
    int id;                     // ID
    int p;                      // Processor count
    std::queue<uint8_t> q;      // Queue for permutations
    bool early_exit = false;    // Early exit for grads
    bool *ee_ptr = &early_exit;
    bool print_out = false;
    uint32_t count = 0;
    uint32_t global_count = 0;

    MPI_Init(&argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    try{
        n = stoi(std::string(argv[1]));
        if (n > 19){
            if (id == 0)
                std::cout << "\nYour n: " << argv[1] << ", is too large. " << 
                "Please pick a number under 19. Aborting...\n" << std::endl;
            MPI_Finalize();
            return 0;
        }
        else{
            user_defined = true;
        }

    }catch(std::exception const & e){
        if (id == 0){
            std::string no_n = "basic_string::_M_construct null not valid";
            if (std::string(e.what()) != no_n){
                std::cout << "An error occured: " << e.what() 
                          << ", aborting..." << std::endl;
                MPI_Finalize();
                return 0;
            }
        }
    }



    

#ifdef DEBUG
    singleTest();
    return 0;
#endif

// Example MPI code:
// https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node72.html
// MPI_Comm comm; 
//     int gsize,*sendbuf; 
//     int root, rbuf[100]; 
//     ... 
//     MPI_Comm_size( comm, &gsize); 
//     sendbuf = (int *)malloc(gsize*100*sizeof(int)); 
//     ... 
//     MPI_Scatter( sendbuf, 100, MPI::UNSIGNED_CHAR, rbuf, 100, MPI::UNSIGNED_CHAR, root, comm); 


    if (user_defined){
        // Run "n" queens on the N passed in
        if (id != 0){
            // std::cout << "myrank: " << id << std::endl;
            uint8_t *queens = (uint8_t*)malloc(n * sizeof(uint8_t));
            for (uint8_t i = 0; i < n; ++i){
                queens[i] = i;
            }
            uint64_t rank_perms = factorials[n] / p;
            if (id != 1){
                // Skip this for id == 1
                uint64_t id_rank_perms = rank_perms * id + factorials[n] % p;
                nthPermutation(n, id_rank_perms, queens);
            }
            else{
                rank_perms = (factorials[n] / p) + (factorials[n] % p);
            }
            // std::cout << "Permutations: " << rank_perms << std::endl;
            

            Board b(rank_perms, n, print_out, queens, ee_ptr);
            count = b.validBoardCount();
            // count = rank_perms;
            // std::cout << "rank[" << id << "]: " << count << std::endl;
            free(queens);
        }
    }

    MPI_Reduce(&count, &global_count, 1, MPI::UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if (id == 0){
        std::cout << "Running " << int(n) << "-queens..." << std::endl;
        std::cout << "Valid queen positions: " << global_count << std::endl;
    }

    // else{
    //     if (id == 0){
    //         std::cout << "Running " << 0 << "-queens..." << std::endl;
    //         std::cout << "Valid queen positions: " << count << std::endl;
    //     }
    //     for(uint8_t i = 8; i < 9; ++i){
    //         // Run each "n" queens
    //         if (p < 8 || factorials[i] < p){
    //             // Run sequentially
    //             std::cout << "Running id: " << id << std::endl;
    //             if (id == 0){
    //                 std::cout << "Running " << int(i) << "-queens..." << std::endl;
    //                 uint8_t *queens = (uint8_t*)malloc(i * sizeof(uint8_t));
    //                 for (uint8_t j = 0; j < i; ++j){
    //                     queens[j] = j;
    //                 }
    //                 Board b(factorials[i], i, print_out, queens, ee_ptr);
    //                 count = b.validBoardCount();
    //                 std::cout << "Valid queen positions: " << count << std::endl;
    //                 free(queens);
    //             }
    //         }
    //         else{
    //             std::cout << "Switching to MPI..." << std::endl;
    //             uint32_t global_count = 0;
    //             if (id != 0){
    //                 // uint32_t permutations = factorials[i];
    //                 uint32_t rank_perms = factorials[i] / p;
    //                 uint32_t leftover = factorials[i] % p;
    //                 std::cout << "Running " << int(i) << "-queens..." << std::endl;
    //                 uint8_t *queens = (uint8_t*)malloc(i * sizeof(uint8_t));
    //                 // MUST initialize queens array
    //                 for (uint8_t j = 0; j < i; ++j){
    //                     queens[j] = j;
    //                 }
    //                 // Fill queens with correct permutation to start at based on rank
    //                 if (id == 1){
    //                     rank_perms += leftover;
    //                     nthPermutation(i, rank_perms, queens);
    //                 }
    //                 else{
    //                     nthPermutation(i, rank_perms * id, queens);
    //                 }

    //                     std::cout << "permutations: " << rank_perms << std::endl;
    //                 // if (rank_perms != 0){
    //                     Board b(rank_perms, i, print_out, queens, ee_ptr);
    //                     b.printOut();
    //                     count = b.validBoardCount();
    //                 // }
    //                 std::cout << "Valid queen positions[" << id << "]: " << count << std::endl;
    //                 free(queens);
    //             }
    //             else{
    //                 // MPI_Barrier(MPI_COMM_WORLD);
    //                 // MPI_Reduce(&count, &global_count, 1, MPI::UNSIGNED_LONG, MPI_SUM, 0 , MPI_COMM_WORLD);
    //                 // std::cout << "Valid queen positions: " << global_count << std::endl;
    //             }
    //         }
    //         MPI_Barrier(MPI_COMM_WORLD);
    //     }
    // }

    MPI_Finalize();
    return 0;
}

