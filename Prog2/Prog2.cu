#include <iostream>     // cout
#include <stdint.h>     // Standardized integers


const uint16_t BLK_SIZE = 256;
void sanityCheck();
bool arrayCheck(const float *a1, const float *a2, const uint32_t &size);

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
    float expected[n] = {68.0f, 167.0f, 266.0f};
    float result[n] = {0.0f, 0.0f, 0.0f};

    // Call CUDA function here...

    if (arrayCheck(expected, result, n)){
        std::cout << "Sanity check PASSED!" << std::endl;
    }
    else{
        std::cout << "Sanity check FAILED!" << std::endl;
    }
}



bool arrayCheck(const float *a1, const float *a2, const uint32_t &size){
    for (uint32_t i = 0; i < size; ++i){
        if (a1[i] != a2[i]){
            return false;
        }
    }
    return true;
}