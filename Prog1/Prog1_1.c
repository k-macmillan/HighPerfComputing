#include <inttypes.h>   // Better integer functionality
#include <stdio.h>      // Printing to console
#include <omp.h>        // Multithreading



/* Return 1 i
f 'i'th bit of 'n' is 1; 0 otherwise */
#define EXTRACT_BIT(n,i) ((n&(1<<i))?1:0)
/* Check if a given input produces TRUE (a one) */
int check_circuit (int id, int z) 
{
int v[16];        /* Each element is a bit of z */
int i;
for (i = 0; 
i < 16; i++) v[i] = EXTRACT_BIT(z,i);
if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
&& (!v[3] || !v[4]) && (v[4] || !v[5])
&& (v[5] || !v[6]) && (v[5] || v[6])
&& (v[6] || !v[15]) && (v[7] || !v[8])
&& (!v[7] || !v[13]
) && (v[8] || v[9])
&& (v[8] || !v[9]) && (!v[9] || !v[10])
&& (v[9] || v[11]) && (v[10] || v[11])
&& (v[12] || v[13]) && (v[13] || !v[14])
&& (v[14] || v[15])) {
printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d
\
n", id,
v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],
v[10],v[11],v[12],v[13],v[14],v[15]);
fflush (stdout);
return 1;
} else return 0;
}