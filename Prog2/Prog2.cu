#include <iostream>     // cout
#include <stdint.h>     // Standardized integers
#include <unistd.h>


const uint16_t BLK_SIZE = 3;

void sanityCheck();
bool arrayCheck(const float *a1, const float *a2, const uint16_t &size);
bool matrixCheck(float **m1, float **m2, const uint16_t &size);
void printArray(const float *arr, const uint16_t &size);
void printMatrix(float **arr, const uint16_t &size);

__global__ void matDotVec(float **arr, const uint16_t &size);

int main(int argc, char **argv){
    sanityCheck();
    return 0;
}



void sanityCheck()
{
    uint16_t n = BLK_SIZE;

    float **ptr = NULL;
    float testnxn[BLK_SIZE][BLK_SIZE];
    ptr = testnxn;
    float incr = 0.0;
    for (uint16_t i = 0; i < n; ++i){
        for (uint16_t j = 0; j < n; ++j){
            testnxn[i][j] = ++incr;
        }
    }
    float testnx1[n] = {10.0f, 11.0f, 12.0f};

    float *d_result;
    float **h_result;
    float *expected;
    uint32_t bytes = n * sizeof(float);
    uint32_t bytes_sqrd = bytes * bytes;

    h_result = (float**)malloc(bytes_sqrd);
    memset(h_result, 0.0f, bytes_sqrd);

    expected = (float*)malloc(bytes);
    expected[0] = 68.0f;
    expected[1] = 167.0f;
    expected[2] = 266.0f;
    

    cudaMalloc((float**)&d_result, bytes_sqrd);
    cudaMemcpy(d_result, testnxn, bytes_sqrd, cudaMemcpyHostToDevice);
    // cudaMemcpy(d_result, expected, bytes, cudaMemcpyHostToDevice);

    // Call CUDA function here...
    matDotVec<<<1, n>>>(ptr, n);

    cudaMemcpy(h_result, d_result, bytes_sqrd, cudaMemcpyDeviceToHost);
    cudaFree(d_result);

    if (matrixCheck(h_result, h_result, n)){
        std::cout << "Sanity check PASSED!" << std::endl;
    }
    else{
        std::cout << "Sanity check FAILED!" << std::endl;
        
    }
    printMatrix(h_result, n);
    free(h_result);
    free(expected);
}



bool arrayCheck(const float *a1, const float *a2, const uint16_t &size){
    for (uint16_t i = 0; i < size; ++i){
        if (a1[i] != a2[i]){            
            return false;
        }
    }
    return true;
}



bool matrixCheck(float **m1, float **m2, const uint16_t &size){
    for (uint16_t i = 0; i < size * size; ++i){
        if (m1[i] != m2[i]){            
            return false;
        }
    }
    return true;
}



void printArray(const float *arr, const uint16_t &size){
    for (uint16_t i = 0; i < size; ++i){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}



void printMatrix(float **arr, const uint16_t &size){
    for (uint16_t i = 0; i < size; ++i){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}


__global__ void matDotVec(float **arr, const uint16_t &size){
    
}