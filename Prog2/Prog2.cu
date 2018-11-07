#include <iostream>     // cout
#include <stdint.h>     // Standardized integers
#include <unistd.h>


const uint16_t BLK_SIZE = 256;
void sanityCheck();
bool arrayCheck(const float *a1, const float *a2, const uint32_t &size);
void printArray(const float *a1, const uint32_t &size);

__global__ void matDotVec(const float *a1, const uint32_t &size);

int main(int argc, char **argv){
    sanityCheck();
    return 0;
}



void sanityCheck()
{
    const uint32_t n = 3;
    float testnxn[n][n];
    float incr = 0.0;
    for (uint8_t i = 0; i < n; ++i){
        for (uint8_t j = 0; j < n; ++j){
            testnxn[i][j] = ++incr;
        }
    }
    float testnx1[n] = {10.0f, 11.0f, 12.0f};

    float *d_result;
    float *h_result;
    float *expected;
    uint32_t bytes = n * sizeof(float);

    h_result = (float*)malloc(bytes);
    memset(h_result, 0.0f, bytes);

    expected = (float*)malloc(bytes);
    expected[0] = 68.0f;
    expected[1] = 167.0f;
    expected[2] = 266.0f;
    

    cudaMalloc((float**)&d_result, bytes);
    cudaMemcpy(d_result, h_result, bytes, cudaMemcpyHostToDevice);

    // Call CUDA function here...
    matDotVec<<<1, n>>>(n);

    cudaMemcpy(h_result, d_result, bytes, cudaMemcpyDeviceToHost);
    cudaFree(d_result);

    if (arrayCheck(expected, h_result, n)){
        std::cout << "Sanity check PASSED!" << std::endl;
    }
    else{
        std::cout << "Sanity check FAILED!" << std::endl;
        printArray(h_result, n);
    }
    
    free(h_result);
    free(expected);
}



bool arrayCheck(const float *a1, const float *a2, const uint32_t &size){
    for (uint32_t i = 0; i < size; ++i){
        if (a1[i] != a2[i]){            
            return false;
        }
    }
    return true;
}



void printArray(const float *a1, const uint32_t &size){
    for (uint32_t i = 0; i < 3; ++i){
        std::cout << a1[i] << " ";
    }
    std::cout << std::endl;
}



__global__ void matDotVec(const float *a1, const uint32_t &size){
    __syncthreads();
}