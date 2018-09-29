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


void Printout(bool* visited, uint32_t* integers, uint32_t size) {

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
    printf("\n");
}


int main(int argc, char **argv){
    if (argc != 2){
        Usage(argv[0]);
    }
    uint32_t n = strtoul(argv[1], NULL, 10) + 1u;
    if (n > 10000000){
        Usage(argv[0]);
    }

    // Initialize variables and allocate arrays
    uint32_t p = 2u;                // Represents a prime number
    double begin = 0.0;
    double end = 0.0;
    double time = 0.0;
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
    while (p < n - 1){
        begin = omp_get_wtime();
        #pragma omp for schedule(static)
        for (uint32_t i = p + p; i < n; i += p){
            visited[i] = true;  // Just set it regardless
        }
        end = omp_get_wtime();
        time += (end - begin) * 1000.0;

        // Cannot OMP this one...could store lowest for each an then find lowest in a critical step
        for (uint32_t i = p + 1; i < n; ++i){
            
            if (visited[i] == false){
                p = i;
                break;
            }
            p = i;
        }
    }    


#else
    // No OMP version
    while (p < n - 1){
        begin = omp_get_wtime();
        for (uint32_t i = p + p; i < n; i += p){
            visited[i] = true;  // Just set it regardless
        }
        end = omp_get_wtime();
        time += (end - begin) * 1000.0;
        // Cannot OMP this one...could store lowest for each an then find lowest in a critical step
        for (uint32_t i = p + 1; i < n; ++i){
            
            if (visited[i] == false){
                p = i;
                break;
            }
            p = i;
        }
    }

#endif

    Printout(visited, integers, n);
    return 0;
}