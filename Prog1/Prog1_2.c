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
 * In arg:    primes[], integers[], size
 */
void Printout(bool *primes, uint32_t size, double time, uint8_t runs) {
    // Printing count variables
    uint8_t count = 0;
    int32_t p_count = 0;

    for (int32_t i = 2; i < size; ++i){
        if (count == 0){
            printf("\n%*" PRIu32 ": ",6, p_count);
            p_count += 10;
            count = 10;
        }
        if (primes[i] == true){
            printf("%" PRIu32 " ", i);
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

    // Don't allow users to go higher than 10,000,000
    if (n > 10000001){
        Usage(argv[0]);
    }

    // Timing
    double begin = 0.0;
    double end = 0.0;
    double time = 0.0;
    uint8_t runs = 20;

    // Primes array information
    uint32_t size = sqrt(n) + 1;
    bool *primes = (bool*) malloc(n * sizeof(bool));               //memory allocated using malloc
    
    
#ifdef OMP
    // OMP version
    omp_set_nested(1);
    for (uint8_t k = 0; k < runs; ++k){
        // Reset array
        for (uint32_t i = 0; i < n; ++i){
            primes[i] = true;
        }

        begin = omp_get_wtime();
        #pragma omp parallel for num_threads(8) schedule(dynamic)
        for (uint32_t i = 2; i < size; ++i){
            if (primes[i]){
                for (uint32_t j = i * i; j < n; j += i){
                    primes[j] = false;
                }
            }
        }
        end = omp_get_wtime();
        time += (end - begin) * 1000.0;
    }


#else
    // No OMP version
    for (uint8_t k = 0; k < runs; ++k){
        // Reset array
        for (uint32_t i = 0; i < n; ++i){
            primes[i] = true;
        }

        begin = omp_get_wtime();
        for (uint32_t i = 2; i < size; ++i){
            if (primes[i]){
                for (uint32_t j = i * i; j < n; j += i){
                    primes[j] = false;
                }
            }
        }
        end = omp_get_wtime();
        time += (end - begin) * 1000.0;        
    }

#endif

    Printout(primes, n, time, runs);
    free(primes);
    return 0;
}