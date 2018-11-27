// THIS IS COPY/PASTA OF CIRCUITS.C FROM DR. KARLSSON TO TEST MPICC

#include <mpi.h>
#include <stdio.h>
// The following macro is the value of the ith bit of n. It does
// a bitwise-and of n and the number 0 everywhere except for the
// ith bit (defining the 0 bit as the least signficiant bit.
#define EXTRACT_BIT(n, i) ( ((n)&(1 << (i)) ) ?1:0)
// check circuit ( id ,n) checks whether inputval encodes a
// 16-bit input for the circuit , and if it does it prints the
// 16 bits along withprocess id of the process that executed it.
void check_circuit(int proc_id, int inputval);
int main (int argc, char* argv[]){
  int i;
  int id;
  int p;
  MPI_Init(&argc, &argv );
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  for (i = id; i < 65536; i += p)
    check_circuit(id, i);
  printf("Process %d has finished \n", id);
  fflush(stdout);
  MPI_Finalize();
  return 0;
}
// circuitvalue (v, len) evaluates the set of 16 bits
// stored in array v [0..15] and returns 1 if the circuit
// is true and 0 if it is false.
int circuitvalue(short v[ ]) {
  return(
     (( v[0]  ||  v[1])  && (!v[1]  ||  v[3]))
  && (( v[2]  ||  v[3])  && (!v[3]  || !v[4]))
  && (( v[4]  || !v[5])  && ( v[5]  ||  v[6]))
  && (( v[5]  || !v[6])  && ( v[7]  || !v[8]))
  && (( v[8]  ||  v[9])  && ( v[8]  || !v[9]))
  && ((!v[9]  || !v[10]) && ( v[10] ||  v[11]))
  && (( v[11] ||  v[9])  && ( v[12] ||  v[13]))
  && ((!v[8]  || !v[13]) && ( v[13] || !v[14]))
  && (( v[14] ||  v[15]) && (!v[15] ||  v[6]))
  );
}
void check_circuit(int proc_id, int inputval) {
  short v[16];
  int i;
  for (i = 0; i < 16; i++)
    v[i] = EXTRACT_BIT (inputval, i);
  if(circuitvalue(v)) {
    printf ("%d: %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", proc_id,
      v[0], v [1], v [2], v[3], v[4], v[5], v[6], v[7], v[8],
      v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
    fflush(stdout);
  }
}
