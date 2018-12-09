#ifndef __NTHPERMUTATION_H_
#define __NTHPERMUTATION_H_

#include <stdint.h>     // better integers
#include "completion.h" // factorials[]

// https://stackoverflow.com/a/7919887  ith permutation


void nthPermutation(const uint8_t &size, uint32_t n, uint8_t *perm )
{
   uint8_t j, k = 0;

   // compute factorial code
   for (k = 0; k < size; ++k)
   {
      perm[k] = n / factorials[size - 1 - k];
      n = n % factorials[size - 1 - k];
   }

   // readjust values to obtain the permutation
   // start from the end and check if preceding values are lower
   for (k = size - 1; k > 0; --k)
      for (j = k - 1; j >= 0; --j)
         if (perm[j] <= perm[k])
            perm[k]++;
}


#endif