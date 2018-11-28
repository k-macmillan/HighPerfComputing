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

    MPI_Finalize();
    return 0;
}