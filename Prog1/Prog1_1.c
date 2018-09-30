#include <inttypes.h>   // Better integer functionality
#include <stdio.h>      // Printing to console
#include <omp.h>        // Multithreading
#include <stdbool.h>    // Booleans


uint8_t EXTRACT_BIT(uint16_t n, uint8_t i);
void PrintV(uint8_t *v);

int main(int argc, char **argv){
    uint16_t arg = 65535u;
    uint8_t v[16];

    for (uint8_t i = 0; i < 16; ++i){
        v[i] = EXTRACT_BIT(arg, i);
    }

    PrintV(v);
}

void PrintV(uint8_t *v){
    for (uint8_t i = 0; i < 16; ++i){
        printf("v[%" PRIu8 "]: %" PRIu8 "\n", i, v[i]);
    }
}

/* Check if a given input produces TRUE (a one) */
int check_circuit (int id, int z){
    int v[16];        /* Each element is a bit of z */
    int i;
    for (i = 0; i < 16; i++){
        v[i] = EXTRACT_BIT(z,i);
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
uint8_t inline EXTRACT_BIT(uint16_t n, uint8_t i){return n & 1 << i ? 1 : 0;}