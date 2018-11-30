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

#include "completion.h" // Completeness function/array
#include "board.h"


int main (int argc, char** argv){
    // Can you do argument error checking with MPI?
    uint32_t N = atoi(argv[1]);
    int id;    // ID
    int p;     // Processor
    MPI_Init(&argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    uint32_t n = 5;
    uint32_t queens[6] = {4, 1, 5, 0, 2, 3 };
    Board test = Board(n, queens);
    bool temp = test.validBoard();
    if (temp){
        // std::cout << "TRUE" << std::endl;
        std::cout << "{";
        for (uint32_t i = 0; i <= n; ++i){
            std::cout << queens[i] << ", ";
        }
        std::cout << "}" << std::endl;
        
    }
    return 0;
    // uint32_t sum = 15;
    uint32_t row_count = 0;
    uint32_t count = 0;
    for (uint32_t a = 0; a <= n; ++a){
        for (uint32_t b = 0; b <= n; ++b){
            for (uint32_t c = 0; c <= n; ++c){
                for (uint32_t d = 0; d <= n; ++d){
                    for (uint32_t e = 0; e <= n; ++e){
                        for (uint32_t f = 0; f <= n; ++f){
                            queens[0] = a;
                            queens[1] = b;
                            queens[2] = c;
                            queens[3] = d;
                            queens[4] = e;
                            queens[5] = f;
                            Board test = Board(n, queens);
                            bool temp = test.validBoard();
                            if (temp){
                                // std::cout << "TRUE" << std::endl;
                                std::cout << "{";
                                for (uint32_t i = 0; i <= n; ++i){
                                    std::cout << queens[i] << ", ";
                                }
                                std::cout << "}" << std::endl;
                                ++count;
                                
                            }
                            if (test.validRow()){
                                    ++row_count;
                            }
                            // else{
                            //     std::cout << "FALSE" << std::endl;
                            // }
                        }
                    }
                }
            }
        }
    }
    
    std::cout << "Count: " << count << std::endl;
    std::cout << "Row Count: " << row_count << std::endl;
    
    

    MPI_Finalize();
    return 0;
}