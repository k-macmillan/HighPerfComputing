/* File:     Prog1_1.c
 * Purpose:  Find all primes up to a given integer
 *
 *              0: 2 3 5 7 11 13
 *
 * Compile:  gcc -Wall -fopenmp -o p1 Prog1_1.c
 * Run:      ./p2
 *
 * Output:   Number of 16 bit combinations that output a 1
 *
 * Notes:
 *
 */        
#include <inttypes.h>   // Better integer functionality
#include <stdio.h>      // Printing to console
#include <omp.h>        // Multithreading


uint8_t check_circuit (int id, uint16_t z);
uint16_t EXTRACT_BIT(uint16_t n, uint16_t i);


/*------------------------------------------------------------------
 * Function:  main
 * Purpose:   Runs 65536 combinations against a circuit, counting the number 
 *            that output a 1. Also prints out any bit combinations that 
 *            produce a 1.
 *
 * In arg:    argc, argv
 */
int main(int argc, char **argv){
    const uint16_t MAX_VAL = 65535;
    uint16_t sum = 0;

    // Timings
    double begin = 0.0;
    double end = 0.0;
    double time = 0.0;
    uint16_t runs = 1;

    begin = omp_get_wtime();
    for (uint16_t k = 0; k < runs; ++k){
        #pragma omp parallel for num_threads(omp_get_num_procs())\
                                 schedule(static, 1)\
                                 reduction(+: sum)
        for (uint32_t i = 0; i < MAX_VAL; ++i){
            sum += check_circuit(omp_get_thread_num(), i);
        }
    }
    end = omp_get_wtime();
    time = (end - begin) * 1000;
    printf("Found %" PRIu16 " valid combinations in %f ms.\n", sum / runs, time / runs);
}


/*------------------------------------------------------------------
 * Function:  check_circuit
 * Purpose:   Checks if the input integer: z will yield a one from the circuit.
 * In arg:    id, z
 * Out arg:   1 or 0
 */
uint8_t check_circuit (int id, uint16_t z){
    uint8_t v[16];

    for (uint8_t i = 0; i < 16; ++i){
        v[i] = EXTRACT_BIT(z, i);
    }
    if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) && 
        (!v[3] || !v[4]) && (v[4] || !v[5]) && (v[5] || !v[6]) && 
        (v[5] || v[6]) && (v[6] || !v[15]) && (v[7] || !v[8]) && 
        (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9]) && 
        (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11]) && 
        (v[12] || v[13]) && (v[13] || !v[14]) && (v[14] || v[15])) {
        printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",
                id,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],
                v[9],v[10],v[11],v[12],v[13],v[14],v[15]);
        fflush (stdout);
        return 1;
    } else{
        return 0;
    }
}


/*------------------------------------------------------------------
 * Function:  EXTRACT_BIT
 * Purpose:   Shifts 1 i bits over and ands it with n, giving a 1 or 0
 * In arg:    n, i
 * Out arg:   1 or 0
 */
uint16_t inline EXTRACT_BIT(uint16_t n, uint16_t i){return n & 1 << i ? 1 : 0;}