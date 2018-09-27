/* File:    problem3.cpp
 * Purpose: Testing of each subproblem in Problem 3.
 *
 * Compile: g++ -Wall -fopenmp -o problem3 problem3.cpp -std=c++11
            g++ -Wall -fopenmp -o problem3 problem3.cpp -DDEBUG -std=c++11
 * Run:     ./problem3 
 *
 * Input:   none
 * Output:  Run times for each of the three runs
 *
 * Notes:
 *    1.    Uncomment the one you want to run
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
#ifdef DEBUG
    int64_t x = 6400;
#else
    int64_t x = 34359738368;
#endif
high_resolution_clock::time_point t1 = high_resolution_clock::now();
high_resolution_clock::time_point t2 = high_resolution_clock::now();
duration<double> run_time = duration_cast<duration<double>>\
    (high_resolution_clock::now() - high_resolution_clock::now());


void problem3_a();
void problem3_b();
void problem3_c();
void problem3_d();
void problem3_e();
void problem3_f();
void problem3_g();
void problem3_h();
int64_t foo(int64_t i);

int main(int argc, char* argv[]){
    
    printf("\nProblem 3a:\n");
    problem3_a();
    printf("\nProblem 3b:\n");
    // problem3_b();
    printf("Cannot be done in parallel as is.\n");
    printf("\nProblem 3c:\n");
    problem3_c();
    printf("\nProblem 3d:\n");
    problem3_d();
    printf("\nProblem 3e:\n");
    problem3_e();
    printf("\nProblem 3f:\n");
    problem3_f();
    printf("\nProblem 3g:\n");
    problem3_g();
    printf("\nProblem 3h:\n");
    problem3_h();

    printf("\nDone with problem 3...\n");
    return 0;
}

void problem3_a(){
    
    uint64_t len = (long) sqrt(x);
    uint64_t a[len] = {};
    uint64_t b[len] = {};

    int64_t i = 0;
    

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i = 0; i < (long) sqrt(x); ++i) {
        a[i] = 2.3 * i;
        if (i < 10) 
            b[i] = a[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < (long) sqrt(x); ++j){
#ifdef DEBUG
        printf("a[%" PRIi64"]: %" PRIu64 "  \tb[%" PRIi64 "]: %" PRIu64 "\n", j, a[j], j, b[j]);
#endif
        a[j] = 0;
        b[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation
    #pragma omp for
    for(i = 0; i < (long) sqrt(x); ++i) {
        a[i] = 2.3 * i;
        if (i < 10) 
            b[i] = a[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < (long) sqrt(x); ++j){
#ifdef DEBUG
        printf("a[%" PRIi64"]: %" PRIu64 "  \tb[%" PRIi64 "]: %" PRIu64 "\n", j, a[j], j, b[j]);
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
    int64_t n = x;
    uint64_t a[n] = {};
    uint64_t b[n] = {};

    int64_t i = 0;
    int8_t flag = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i = 0; (i * flag) < n; ++i) {
        a[i] = 2.3 * i;
        if ( a[i] < b[i])
            flag = 1;
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIu64"]: %" PRIu64 "  \tb[%" PRIu64 "]: %" PRIu64 "\n", j, a[j], j, b[j]);
#endif
        a[j] = 0;
        b[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation
    #pragma omp for
    for(i = 0; i < n; ++i) {
        a[i] = 2.3 * i;
        if ( a[i] < b[i])
            flag = 1;
    }

    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64"]: %" PRIu64 "  \tb[%" PRIi64 "]: %" PRIu64 "\n", j, a[j], j, b[j]);
#endif
        a[j] = 0;
        b[j] = 0;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}


int64_t foo(int64_t i){
    return i + i;
}


void problem3_c(){
    // c.
    // for(i=0; i < n; ++i) 
        // a[i] = foo(i);
#ifdef DEBUG    
    int64_t n = 12;
#else
    int64_t n = 600000;
#endif
    uint64_t a[n] = {};
    int64_t i = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i = 0; i < n; ++i) {
        // printf("i: %" PRIi64 "\n", i);
        a[i] = foo(i);
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIu64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation
    #pragma omp for
    for(i = 0; i < n; ++i) {
        a[i] = foo(i);
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIu64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}




void problem3_d(){
// d.
// for(i=0; i<n; i++) {
//     a[i] = foo(i);
//     if(a[i] < b[i]) 
//         a[i] = b[i];
//     }
#ifdef DEBUG    
    int64_t n = 12;
#else
    int64_t n = 60000;
#endif
    uint64_t a[n] = {};
    uint64_t b[n] = {};
    int64_t i = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i = 0; i < n; ++i) {
        a[i] = foo(i);
        if(a[i] < b[i]) 
            a[i] = b[i];
        }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIu64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation
    #pragma omp for
    for(i = 0; i < n; ++i) {
        a[i] = foo(i);
        if(a[i] < b[i])
            a[i] = b[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIu64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}



void problem3_e(){
// e.
// for(i = 0; i < n; i++) {
//     a[i] = foo(i);
//     if(a[i] < b[i])
//         break;
// }
#ifdef DEBUG    
    int64_t n = 12;
#else
    int64_t n = 60000;
#endif
    uint64_t a[n] = {};
    uint64_t b[n] = {};
    int64_t i = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i = 0; i < n; ++i) {
        a[i] = foo(i);
        if(a[i] < b[i])
            break;
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIu64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation
    #pragma omp for
    for(i = 0; i < n; ++i) {
        a[i] = foo(i);
        if(a[i] < b[i])
            // break;   // Have to comment out or can't compile!
            continue;
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIu64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}



void problem3_f(){
// f.
// dotp = 0;
// for(i = 0; i < n; i++)
//     dotp += a[i] * b[i];
#ifdef DEBUG    
    int64_t n = 12;
#else
    int64_t n = 60000;
#endif
    uint64_t a[n] = {};
    uint64_t b[n] = {};
    int64_t i = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    int64_t dotp = 0;
    for(i = 0; i < n; i++){
        dotp += a[i] * b[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIi64 "\n", j, dotp);
#endif
        a[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    t1 = high_resolution_clock::now();
    // OMP implementation    
    dotp = 0;
    #pragma omp for // reduction(+: dotp)
    for(i = 0; i < n; i++){
        dotp += a[i] * b[i];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIi64 "\n", j, dotp);
#endif
        a[j] = 0;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}



void problem3_g(){
// g.
// for(i=k; i<2*k; i++)
//     a[i] = a[i] + a[i-k];
#ifdef DEBUG    
    int64_t n = 12;
    int64_t k = 2;
#else
    int64_t n = 600000;
    int64_t k = 200000;
#endif
    uint64_t a[n] = {};
    int64_t i = 0;

    // Setup array
    for(i = 0; i < n; ++i){
        a[i] = i + 1;
    }
    i = 4;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i = k; i < 2 * k; ++i){
        a[i] = a[i] + a[i-k];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIi64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    // Setup array
    for(i = 0; i < n; ++i){
        a[i] = i + 1;
    }
    i = 4;

    t1 = high_resolution_clock::now();
    // OMP implementation     
    #pragma omp for
    for(i = k; i < 2 * k; ++i){
        a[i] = a[i] + a[i-k];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIi64 "\n", j, a[j]);
#endif
        a[j] = j;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}



void problem3_h(){
// h.
// for(i = k; i < n; i++)
//     a[i] = b * a[i-k];
#ifdef DEBUG    
    int64_t n = 12;
#else
    int64_t n = 600000;    
#endif
    uint64_t a[n] = {};
    int64_t i = 0;
    int64_t k = 2;
    int64_t b = 5;

    // Setup array
    for(i = 0; i < n; ++i){
        a[i] = i + 1;
    }
    i = 0;

    // Sequential sanity check
    t1 = high_resolution_clock::now();
    for(i = k; i < n; ++i){
        a[i] = b * a[i-k];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIi64 "\n", j, a[j]);
#endif
        a[j] = 0;
    }
    printf("No OMP time : %.14f seconds\n", run_time.count());

    // Setup array
    for(i = 0; i < n; ++i){
        a[i] = i + 1;
    }
    i = 0;

    t1 = high_resolution_clock::now();
    // OMP implementation     
    #pragma omp for
    for(i = k; i < n; ++i){
        a[i] = b * a[i-k];
    }
    t2 = high_resolution_clock::now();
    run_time = duration_cast<duration<double>>(t2 - t1);
    for(int64_t j = 0; j < n; ++j){
#ifdef DEBUG
        printf("a[%" PRIi64 "]: %" PRIi64 "\n", j, a[j]);
#endif
        a[j] = j;
    }
    printf("OMP time    : %.14f seconds\n", run_time.count());
}

