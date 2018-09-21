/* File:    problem3.cpp
 * Purpose: Alternates sign of integer added to sum
 *
 *              sum = 0 + 1 + -2 + 3 + -4...
 *
 * Compile: g++ -Wall -fopenmp -o problem3 problem2.cpp -std=c++11 -lm
            g++ -Wall -fopenmp -o problem3 problem2.cpp -DDEBUG -std=c++11
 * Run:     ./problem3 
 *
 * Input:   none
 * Output:  Times for each of the three runs
 *
 * Notes:
 *    1.    You probably want to reduce x if running in DEBUG mode.
 *
 */

#include <inttypes.h>   // Better integer functionality
#include <stdio.h>      // Printing to console
#include <math.h>       // sqrt
#include <omp.h>        // Multithreading
#include <chrono>       // High precision clock

using namespace std::chrono;

// Global
uint8_t     thrds   = omp_get_num_procs();
uint64_t x = 34359738368;
high_resolution_clock::time_point t1 = high_resolution_clock::now();
high_resolution_clock::time_point t2 = high_resolution_clock::now();
duration<double> run_time = duration_cast<duration<double>>\
    (high_resolution_clock::now() - high_resolution_clock::now());


void problem3_a();
void problem3_b();

int main(int argc, char* argv[]){
    
    printf("Problem 3a:\n");
    problem3_a();


    return 0;
}

void problem3_a(){
    
    uint64_t a[(long) sqrt(x)] = {};
    uint64_t b[(long) sqrt(x)] = {};

    uint64_t i = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i=0; i < (long) sqrt(x); i++) {
        a[i] = 2.3 * i;
        if (i < 10) 
            b[i] = a[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(uint64_t j = 0; j < (long) sqrt(x); ++j){
#ifdef DEBUG
        printf("a[%d]: %" PRIu16 "  \tb[%d]: %" PRIu16 "\n", j, a[j], j, b[j]);
#endif
        a[j] = 0;
        b[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation
    #pragma omp for
    for(i=0; i < (long) sqrt(x); i++) {
        a[i] = 2.3 * i;
        if (i < 10) 
            b[i] = a[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(uint64_t j = 0; j < (long) sqrt(x); ++j){
#ifdef DEBUG
        printf("a[%d]: %" PRIu16 "  \tb[%d]: %" PRIu16 "\n", j, a[j], j, b[j]);
#endif
        a[j] = 0;
        b[j] = 0;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}

void problem3_b(){
    // b.
    // flag = 0;
    // for(i = 0; (i<n) && (!flag); i++) {
    //     a[i] = 2.3*i;
    //     if ( a[i] < b[i])
    //         flag = 1;
    // }
    uint64_t a[(long) sqrt(x)] = {};
    uint64_t b[(long) sqrt(x)] = {};

    uint64_t i = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i=0; i < (long) sqrt(x); i++) {
        a[i] = 2.3 * i;
        if (i < 10) 
            b[i] = a[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(uint64_t j = 0; j < (long) sqrt(x); ++j){
#ifdef DEBUG
        printf("a[%d]: %" PRIu16 "  \tb[%d]: %" PRIu16 "\n", j, a[j], j, b[j]);
#endif
        a[j] = 0;
        b[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation
    #pragma omp for
    for(i=0; i < (long) sqrt(x); i++) {
        a[i] = 2.3 * i;
        if (i < 10) 
            b[i] = a[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(uint64_t j = 0; j < (long) sqrt(x); ++j){
#ifdef DEBUG
        printf("a[%d]: %" PRIu16 "  \tb[%d]: %" PRIu16 "\n", j, a[j], j, b[j]);
#endif
        a[j] = 0;
        b[j] = 0;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}


// c.
// for(i=0; i<n; i++) 
// a[i] = foo(i);
// d.
// for(i=0; i<n; i++) {
// a[i] = foo(i);
// if(a[i] < b[i
// ]) a[i] = b[i];
// }
// e.
// for(i=0; i<n; i++) {
// a[i] = foo(i);
// if(a[i] < b[i]) break;
// }
// f.
// dotp = 0;
// for(i=0; i<n; i++)
// dotp += a[i]*b[i];
// g.
// for(i=k; i<2*k; i++)
// a[i] = a[i] + a[i
// -
// k];
// h.
// for(i=k; i<n; i++)
// a[i] = b*a[i
// -
// k];