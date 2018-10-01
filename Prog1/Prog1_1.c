#include <inttypes.h>   // Better integer functionality
#include <stdio.h>      // Printing to console
#include <omp.h>        // Multithreading
#include <stdbool.h>    // Booleans


uint8_t check_circuit (int id, uint16_t z);
void PrintV(uint8_t *v);
uint16_t EXTRACT_BIT(uint16_t n, uint16_t i);


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
        #pragma omp parallel for num_threads(omp_get_num_procs()) schedule(static, 1) reduction(+: sum)
        for (uint32_t i = 0; i < MAX_VAL; ++i){
            sum += check_circuit(omp_get_thread_num(), i);
        }
    }
    end = omp_get_wtime();
    time = (end - begin) * 1000;
    printf("Found %" PRIu16 " valid combinations in %f ms.\n", sum / runs, time / runs);
}

void PrintV(uint8_t *v){
    for (uint8_t i = 0; i < 16; ++i){
        printf("v[%" PRIu8 "]: %" PRIu8 "\n", i, v[i]);
    }
}

/* Check if a given input produces TRUE (a one) */
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