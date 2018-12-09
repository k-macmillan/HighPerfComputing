#ifndef __COMPLETION_H__
#define __COMPLETION_H__

#include <stdint.h>     // uint32_t, uint64_t


uint32_t complete19[19] = { 0, // So we don't have to perform N - 1
                            1, 0, 0, 2, 10, 4,
                            40, 92, 352, 724, 2680, 14200,
                            73712, 365596, 2279184, 14772512, 95815104, 666090624};

uint64_t complete27[27] = { 0, // So we don't have to perform N - 1
                            1, 0, 0, 2, 10, 4,
                            40, 92, 352, 724, 2680, 14200,
                            73712, 365596, 2279184, 14772512, 95815104, 666090624,
                            4968057848, 39029188884, 314666222712, 2691008701644, 24233937684440, 227514171973736,
                            2207893435808350, 22317699619364000};

// 0-16 factorial
uint64_t factorials[17] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600, 6227020800, 87178291200, 1307674368000, 20922789888000};

inline bool correctCount(const uint32_t &N, const uint32_t &count){
    return count == complete19[N];
}

inline bool correctCount(const uint32_t &N, const uint64_t &count){
    return count == complete27[N];
}

inline uint32_t Factorial(uint32_t x) {
  return (x == 1 ? x : x * Factorial(x - 1));
}

#endif