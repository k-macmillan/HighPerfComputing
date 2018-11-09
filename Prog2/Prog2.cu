#include <iostream>     // cout
#include <stdint.h>     // Standardized integers
#include <unistd.h>
#include <math.h>       // fabs


const uint16_t BLK_SIZE = 3;
const float EPSILON = 0.0001;   // For sanity check: float == float

void sanityCheck();
bool resultCheck(float *arr1, float *arr2);
void printArray(float *arr);

__global__ void matDotVec(float **arr, uint16_t size);

int main(int argc, char **argv){
    sanityCheck();
    return 0;
}



void sanityCheck()
{
    uint16_t n = BLK_SIZE;
    uint32_t bytes = n * sizeof(float);
    uint32_t ptr_bytes = n * sizeof(float*);        // Just being safe
    uint32_t bytes_sqrd = bytes * bytes;

    float **h_matrix = (float**)malloc(ptr_bytes);   // To store "Matrix A"
    // float **d_matrix1 = (float*)malloc(ptr_bytes);  // To store the data from host
    // float **d_matrix2 = (float*)malloc(ptr_bytes);  // To store the multiplication that happens
    float *h_vec = (float*)malloc(bytes);           // To store "Vector B" and result
    // float *d_vec = (float*)malloc(bytes);           // To store the data from host and result
    float *expected = (float*)malloc(bytes);        // Sanity check array

    float **d_matrix1;      // To store the data from host
    float **d_matrix2;      // To store the multiplication that happens
    float *d_vec;           // To store the data from host and result

    // Allocate memory
    for (uint16_t i = 0; i < n; ++i){
        h_matrix[i] = (float*)malloc(bytes);
        // d_matrix1[i] = (float*)malloc(bytes);
        // d_matrix2[i] = (float*)malloc(bytes);
        // memset(d_matrix1[i], 0.0f, bytes);
        // memset(d_matrix2[i], 0.0f, bytes);
    }

    float incr = 0.0;
    for (uint16_t i = 0; i < n; ++i){
        for (uint16_t j = 0; j < n; ++j){
            h_matrix[i][j] = ++incr;
        }
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
    cudaMemcpy(d_matrix1, h_matrix, bytes_sqrd, cudaMemcpyHostToDevice);
    cudaMemcpy(d_vec, expected, bytes, cudaMemcpyHostToDevice);

    // Call CUDA function here...
    matDotVec<<<1, n>>>(d_matrix1, n);

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
    for (uint16_t i = 0; i < BLK_SIZE; ++i){
        if (fabs(arr1[i] - arr2[i]) > EPSILON){            
            return false;
        }
    }
    return true;
}




void printArray(float *arr){
    for (uint16_t i = 0; i < BLK_SIZE; ++i){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}


__global__ void matDotVec(float **arr, uint16_t size){
    
}