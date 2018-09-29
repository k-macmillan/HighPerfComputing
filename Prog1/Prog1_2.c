/* File:     Prog1_2.c
 * Purpose:  Find all primes up to a given integer
 *
 *              0: 2 3 5 7 11 13
 *
 * Compile:  gcc -Wall -fopenmp -o p2 Prog1_2.c -lm
 *           gcc -Wall -fopenmp -o p2 -DOMP Prog1_2.c -lm
 * Run:      ./p2 <n>
 *           Where n is the number to check up to, not exceeding 10000000
 *
 * Input:    n            
 * Output:   All prime numbers up to n
 *
 * Notes:
 *    1.     Run with -DOMP to run with OMP
 *
 */        

#include <inttypes.h>   // Better integer functionality
#include <stdbool.h>    // Boolean in C
#include <math.h>       // Square root
#include <stdio.h>      // Printing to console
#include <stdlib.h>     // exit() & strtoull
#include <omp.h>        // Multithreading

const uint32_t UMAX = 4294967295;


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <n>\n", prog_name);  /* Change */
    fprintf(stderr, "   Where n is the number to check up to, not exceeding 10000000\n");
    fprintf(stderr, "example: %s 1000000\n\n", prog_name);
    exit(1);
}  /* Usage */



/*------------------------------------------------------------------
 * Function:  Printout
 * Purpose:   Prints the primes found by the sieve
 * In arg:    visited[], integers[], size
 */
void Printout(bool* visited, uint32_t* integers, uint32_t size, double time, uint8_t runs) {
    // Printing count variables
    uint8_t count = 0;
    uint32_t p_count = 0;

    for (uint32_t i = 0; i < size; ++i){
        if (count == 0){
            printf("\n%" PRIu32 ": ", p_count);
            p_count += 10;
            count = 10;
        }
        if (visited[i] == false){
            printf("%" PRIu32 " ", integers[i]);
            --count;
        }
        
    }
    printf("\nElapsed time = %f ms\n", time / runs);
}



/*------------------------------------------------------------------
 * Function:  main
 * Purpose:   Sets up the problem and runs the sieve
 * In arg:    argc, argv
 */
int main(int argc, char **argv){
    if (argc != 2){
        Usage(argv[0]);
    }
    uint32_t n = strtoul(argv[1], NULL, 10) + 1u;
    if (n > 10000001){
        Usage(argv[0]);
    }

    // Timing
    double begin = 0.0;
    double end = 0.0;
    double time = 0.0;
    uint8_t runs = 10;
    // Initialize variables and allocate arrays
    uint32_t p = 2u;                // Represents a prime number
    bool *visited = (bool*) malloc(n * sizeof(bool));                //memory allocated using malloc
    uint32_t *integers = (uint32_t*) malloc(n * sizeof(uint32_t));   //memory allocated using malloc
    
    // Initialize arrays
    for (uint32_t i = 0; i < n; ++i){
        integers[i] = i;
        visited[i] = false;
    }
    // Ignore 1 and 2
    visited[0] = true;
    visited[1] = true;
    
#ifdef OMP
    // OMP version
    for (uint8_t k = 0; k < runs; ++k){
        while (p < n - 1){
            begin = omp_get_wtime();
            #pragma omp parallel for //num_threads(omp_get_num_procs()) schedule(static, 8)
            for (uint32_t i = p + p; i < n; i += p){
                visited[i] = true;  // Just set it regardless
            }
            

            // Cannot OMP this one...could store lowest for each an then find lowest in a critical step
            uint32_t lowest = UMAX;
            #pragma omp parallel for num_threads(8) //schedule(static, 8) //private(lowest)
            for (uint32_t i = p + 1; i < n; ++i){     
                uint32_t low = UMAX;       
                if (visited[i] == false){
                    low = i;
                }
                #pragma omp critical
                if (low < lowest){
                    lowest = low;
                }
            }
            end = omp_get_wtime();
            time += (end - begin) * 1000.0;
            p = lowest;
        }    
        // Reset sieve
        for (uint32_t i = 0; i < n; ++i){
            integers[i] = i;
            visited[i] = false;
        }
        // Ignore 1 and 2
        visited[0] = true;
        visited[1] = true;
        p = 2u;
    }


#else
        // No OMP version
    for (uint8_t k = 0; k < runs; ++k){
        while (p < n - 1){
            begin = omp_get_wtime();
            for (uint32_t i = p + p; i < n; i += p){
                visited[i] = true;  // Just set it regardless
            }
            
            // Cannot OMP this one...could store lowest for each an then find lowest in a critical step
            for (uint32_t i = p + 1; i < n; ++i){
                
                if (visited[i] == false){
                    p = i;
                    break;
                }
                p = i;
            }
            end = omp_get_wtime();
            time += (end - begin) * 1000.0;
        }
        // Reset sieve
        for (uint32_t i = 0; i < n; ++i){
            integers[i] = i;
            visited[i] = false;
        }
        // Ignore 1 and 2
        visited[0] = true;
        visited[1] = true;
        p = 2u;
    }

#endif

    Printout(visited, integers, n, time, runs);
    return 0;
}