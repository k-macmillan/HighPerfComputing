#include <iostream>     // cout
#include <stdint.h>     // Standardized integers
#include <unistd.h>
#include <math.h>       // fabs
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */


const uint64_t BLK_SIZE = 2048; // 8192
const uint64_t SANITY_N = 3;    // Size of sanity check matrix
const float EPSILON = 0.0001;   // For sanity check: float == float

void testAdotB();
void testSlowAdotB();
void sanityCheck();
bool resultCheck(float *arr1, float *arr2);
void printArray(float *arr);

__global__ void matDotVec(float *m1, float *m2, float *arr, uint64_t size);
__global__ void matAddElementWise(float *m2, float *arr, uint64_t size);
__global__ void slowDotVec(float *m1, float *arr, float *res, uint64_t size);

int main(int argc, char **argv){
    // sanityCheck();
    srand (time(NULL));
    // testAdotB();
    testSlowAdotB();
    return 0;
}


void testAdotB(){
    uint64_t n = BLK_SIZE;
    uint64_t bytes = n * sizeof(float);
    uint64_t ptr_bytes = n * sizeof(float*);        // Just being safe
    uint64_t bytes_sqrd = bytes * bytes;

    float *h_matrix = (float*)malloc(ptr_bytes * ptr_bytes);   // To store "Matrix A"
    float *h_vec = (float*)malloc(bytes);           // To store "Vector B" and result

    float *d_matrix1;       // To store the data from host
    float *d_matrix2;       // To store the multiplication that happens
    float *d_vec;           // To store the data from host and result

    // Fill "Matrix A"
    for (uint64_t i = 0; i < n * n; ++i){
        h_matrix[i] = rand() % 1000;
    }

    // Fill "Vector B"
    for (uint64_t i = 0; i < n; ++i){    
        h_vec[i] = rand() % 1000;
    }

    // Allocate CUDA memory and copy data to device
    cudaMalloc((float**)&d_matrix1, bytes_sqrd);
    cudaMalloc((float**)&d_matrix2, bytes_sqrd);
    cudaMalloc((float**)&d_vec, bytes);
    cudaMemcpy(d_matrix1, h_matrix, bytes_sqrd, cudaMemcpyHostToDevice);
    cudaMemcpy(d_vec, h_vec, bytes, cudaMemcpyHostToDevice);

    int blockSize;
    int minGridSize;
    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, matDotVec, 0, n); 
    uint64_t gridSize = (n + blockSize - 1) / blockSize; 
    matDotVec<<<gridSize, blockSize>>>(d_matrix1, d_matrix2, d_vec, n);
    cudaDeviceSynchronize();    // Just in case
    matAddElementWise<<<gridSize, blockSize>>>(d_matrix2, d_vec, n);
    cudaDeviceSynchronize();


    // Copy result back to host
    cudaMemcpy(h_vec, d_vec, bytes, cudaMemcpyDeviceToHost);


    // Free CUDA and host memory
    cudaFree(d_matrix1);
    cudaFree(d_matrix2);
    cudaFree(d_vec);
    free(h_matrix);
    free(h_vec);
}


void testSlowAdotB(){
    uint64_t n = BLK_SIZE;
    uint64_t bytes = n * sizeof(float);
    uint64_t ptr_bytes = n * sizeof(float*);        // Just being safe
    uint64_t bytes_sqrd = bytes * bytes;

    float *h_matrix = (float*)malloc(ptr_bytes * ptr_bytes);   // To store "Matrix A"
    float *h_vec = (float*)malloc(bytes);           // To store "Vector B" and result

    float *d_matrix1;       // To store the data from host    
    float *d_vec;           // To store the data from host and result
    float *d_vec2;           // To store the data from host and result

    // Fill "Matrix A"
    for (uint64_t i = 0; i < n * n; ++i){    
        h_matrix[i] = rand() % 1000;
    }

    // Fill "Vector B"
    for (uint64_t i = 0; i < n; ++i){    
        h_vec[i] = rand() % 1000;
    }

    // Allocate CUDA memory and copy data to device
    cudaMalloc((float**)&d_matrix1, bytes_sqrd);
    cudaMalloc((float**)&d_vec, bytes);
    cudaMalloc((float**)&d_vec2, bytes);
    cudaMemcpy(d_matrix1, h_matrix, bytes_sqrd, cudaMemcpyHostToDevice);
    cudaMemcpy(d_vec, h_vec, bytes, cudaMemcpyHostToDevice);

    slowDotVec<<<1, 1>>>(d_matrix1, d_vec, d_vec2, n);
    cudaDeviceSynchronize();

    // Copy result back to host
    cudaMemcpy(h_vec, d_vec2, bytes, cudaMemcpyDeviceToHost);


    // Free CUDA and host memory
    cudaFree(d_matrix1);
    cudaFree(d_vec);
    cudaFree(d_vec2);
    free(h_matrix);
    free(h_vec);
}



void sanityCheck()
{
    uint64_t n = SANITY_N;
    uint64_t bytes = n * sizeof(float);
    uint64_t ptr_bytes = n * sizeof(float*);        // Just being safe
    uint64_t bytes_sqrd = bytes * bytes;

    float *h_matrix = (float*)malloc(ptr_bytes * ptr_bytes);   // To store "Matrix A"
    float *h_vec = (float*)malloc(bytes);           // To store "Vector B" and result
    float *expected = (float*)malloc(bytes);        // Sanity check array

    float *d_matrix1;       // To store the data from host
    float *d_matrix2;       // To store the multiplication that happens
    float *d_vec;           // To store the data from host and result
    float *d_vec2;          // To store the data from host and result

    float incr = 0.0;
    for (uint64_t i = 0; i < n * n; ++i){    
        h_matrix[i] = ++incr;
    }

    // "Vector B"
    h_vec[0] = 10.0f;
    h_vec[1] = 11.0f;
    h_vec[2] = 12.0f;
    
    // Expected values out of matrixCheck()
    expected[0] = 68.0f;
    expected[1] = 167.0f;
    expected[2] = 266.0f;
    

    // Allocate CUDA memory and copy data to device
    cudaMalloc((float**)&d_matrix1, bytes_sqrd);
    cudaMalloc((float**)&d_matrix2, bytes_sqrd);
    cudaMalloc((float**)&d_vec, bytes);
    cudaMalloc((float**)&d_vec2, bytes);
    cudaMemcpy(d_matrix1, h_matrix, bytes_sqrd, cudaMemcpyHostToDevice);
    cudaMemcpy(d_vec, h_vec, bytes, cudaMemcpyHostToDevice);

    matDotVec<<<1, n * n>>>(d_matrix1, d_matrix2, d_vec, n);
    cudaDeviceSynchronize();
    matAddElementWise<<<1, n>>>(d_matrix2, d_vec, n);
    cudaDeviceSynchronize();

    // slow test:
    // slowDotVec<<<1, 1>>>(d_matrix1, d_vec, d_vec2, n);
    // cudaDeviceSynchronize();
    // cudaMemcpy(h_vec, d_vec2, bytes, cudaMemcpyDeviceToHost);


    // cudaFree(d_vec);
    // cudaFree(d_vec2);
    // cudaFree(d_matrix1);
    // cudaFree(d_matrix2);
    // end slow test


    // Copy result back to host & free memory
    cudaMemcpy(h_vec, d_vec, bytes, cudaMemcpyDeviceToHost);
    cudaFree(d_vec);
    cudaFree(d_matrix1);
    cudaFree(d_matrix2);

    if (resultCheck(expected, h_vec)){
        std::cout << "Sanity check PASSED!" << std::endl;
    }
    else{
        std::cout << "Sanity check FAILED!" << std::endl;
        
    }
    printArray(h_vec);

    // Free up device memory allocations
    free(h_matrix);
    free(h_vec);
    free(expected);
}



bool resultCheck(float *arr1, float *arr2){
    for (uint64_t i = 0; i < SANITY_N; ++i){
        if (fabs(arr1[i] - arr2[i]) > EPSILON){            
            return false;
        }
    }
    return true;
}




void printArray(float *arr){
    for (uint64_t i = 0; i < SANITY_N; ++i){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}



__global__ void matDotVec(float *m1, float *m2, float *arr, uint64_t size){
    uint64_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size * size){
        m2[idx] = m1[idx] * arr[idx % size];
    }
}

__global__ void matAddElementWise(float *m2, float *arr, uint64_t size){
    uint64_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size * size){
        uint64_t arr_idx = idx % size;
        uint64_t mat_loc = arr_idx * size;
        uint64_t max = mat_loc + size;
        arr[arr_idx] = 0.0f;
        for (uint64_t i = mat_loc; i < max; ++i){
            arr[arr_idx] += m2[i];
        }
    }
}

__global__ void slowDotVec(float *m1, float *arr, float *res, uint64_t size){
    // Assumes this is ran on one thread
    uint64_t row = 0;
    for (uint64_t i = 0; i < size; ++i){
        res[i] = 0.0f;            
        for (uint64_t j = 0; j < size; ++j){
            res[i] += m1[row * size + j] * arr[j];
        }
        ++row;
    }
}