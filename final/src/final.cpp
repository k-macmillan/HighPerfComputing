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
// #include <mpi.h>        // mpi

#include "completion.h" // Completeness function/array
#include "board.h"

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
    // uint32_t N = atoi(argv[1]);
    // int id;    // ID
    // int p;     // Processor
    // MPI_Init(&argc, &argv );
    // MPI_Comm_rank(MPI_COMM_WORLD, &id);
    // MPI_Comm_size(MPI_COMM_WORLD, &p);

#ifdef DEBUG
    singleTest();
    return 0;
#endif

    bool con_out = true;
    uint32_t count = 0;
    uint32_t n = 8;
    uint32_t queens[9] = {0};

    for (uint32_t a = 0; a <= n; ++a){
        for (uint32_t b = 0; b <= n; ++b){
            for (uint32_t c = 0; c <= n; ++c){
                for (uint32_t d = 0; d <= n; ++d){
                    for (uint32_t e = 0; e <= n; ++e){
                        for (uint32_t f = 0; f <= n; ++f){
                            for (uint32_t g = 0; g <= n; ++g){
                                for (uint32_t h = 0; h <= n; ++h){
                                    // Takes about 1 min 43 sec on my laptop for 9x9
                                    // for (uint32_t i = 0; i <= n; ++i){
                                        queens[1] = a;
                                        queens[2] = b;
                                        queens[3] = c;
                                        queens[4] = d;
                                        queens[5] = e;
                                        queens[6] = f;
                                        queens[7] = g;
                                        queens[8] = h;
                                        // queens[9] = i;
                                        Board test = Board(n, queens);
                                        if (test.validBoard()){
                                            ++count;
                                            if (con_out){
                                                std::cout << "{";
                                                for (uint32_t i = 0; i < n; ++i){
                                                    std::cout << queens[i] << ", ";
                                                }
                                                std::cout << queens[n] << "}" << std::endl;
                                            }
                                            
                                        }
                                    // }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::cout << "\nN: " << n << "x" << n << std::endl;
    std::cout << "Count: " << count << std::endl;

    // MPI_Finalize();
    return 0;
}

