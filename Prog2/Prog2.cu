/* File:        Prog2.cu
 *
 * Author:      Kyle MacMillan
 *
 * Assignment:  Programming Assignment #2
 *
 * Course:      CSC 792 - High Performance Computing
 *
 * Purpose:     Tests matrix multiplied with a vector and matrix added to a 
 *              matrix. This was done with a class function because there was a 
 *              lot of repeat code and I like compartmentalizing code. The 
 *              primary functionality of the class is overloaded to accept 
 *              arbitrary matrices/vectors, though they must be of size 
 *              specified at class instantiation. For example you can pass a 
 *              matrix and vector of your choosing to AdotBfast() and it will 
 *              skip the randomization of values.
 *
 *              To use this class simply declare it with the "n" you intend to 
 *              use for the n x n matrix and n x 1 vector. The class will 
 *              generate a random numbers to fill the matrix and vector. If you 
 *              want to pass it your own matrix and vector you can do so by 
 *              calling the appropriate function with the 1D matrix and vector 
 *              you would like to use. It was done this way because it was very 
 *              quick to do and greatly increases the usage range. 
 *
 *              All public functions, other than sanity check, return the 1D 
 *              array result, but is free'd on the next public function call. 
 *              The program intenionally allocates extra memory because I 
 *              wanted to keep the code clean. Extra memory allocation does not 
 *              hamper the performance testing because I did not include that in 
 *              the measurement.
 *
 *              When the class is destructed all allocated memory is freed on 
 *              both the host and device.
 *
 *              Users can specify a matrix/vector size or not, the program will 
 *              run regardless. I set a hard limit of matrix/vector size n being 
 *              1024. It could have been arbitrarily large and increased 
 *              complexity arbitrarily.
 *
 * Compile:     nvcc -o prog2 Prog2.cu -std=c++11
 *              make (with provided Makefil)
 *
 * Run:         ./prog2
 *              ./prog2 #
 *
 * Output:      Santiy check verfication output to screen
 *
 * Notes:       None, runs like a dream
 *
 */        


#include <iostream>     // cout
#include <stdint.h>     // uint64_t
#include <math.h>       // fabs
#include <time.h>       // time
#include <stdlib.h>     // srand, rand



/*------------------------------------------------------------------------------
 * Function:    fastDotVec
 * Purpose:     CUDA function. Stores matrix value multiplied by associated 
 *              array value into m2 for later use.
 *
 * In arg:      m1, m2, arr, size
 *
 * Out arg:     m2
 */
__global__ void fastDotVec(float *m1, float *m2, float *arr, uint64_t size){
    uint64_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size * size){
        m2[idx] = m1[idx] * arr[idx % size];
    }
}



/*------------------------------------------------------------------------------
 * Function:    matAddElementWise
 * Purpose:     CUDA function. Walks N elements of m2, adding each to the 
 *              appropriate index in arr. This is the result of Matrix * Vector.
 *
 * In arg:      m2, arr, size
 *
 * Out arg:     arr
 */
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



/*------------------------------------------------------------------------------
 * Function:    slowDotVec
 * Purpose:     CUDA function. This is to demonstrate running Matrix * Vector on 
 *              a single thread in a CUDA call so we can compare p = 1 to p = n. 
 *              The result is stored in res.
 *
 * In arg:      m1, arr, res, size
 *
 * Out arg:     res
 */
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



/*------------------------------------------------------------------------------
 * Function:    addMatrixFast
 * Purpose:     CUDA function. This is to demonstrate running Matrix + Matrix in 
 *              parallel on a CUDA-capabale card. Simply uses the "location" to 
 *              determine matrix index and add the values together. Result is 
 *              stored in m1.
 *
 * In arg:      m1, m2, size
 *
 * Out arg:     m1
 */
__global__ void addMatrixFast(float *m1, float *m2, uint64_t size){
    uint64_t idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size){
        m1[idx] = m1[idx] + m2[idx];
    }
}



/*------------------------------------------------------------------------------
 * Function:    addMatrixSlow
 * Purpose:     CUDA function. This is to demonstrate running Matrix + Matrix on 
 *              a single thread in a CUDA device. Walks the entire array, adding 
 *              element-wise. Result is stored in m1.
 *
 * In arg:      m1, m2, size
 *
 * Out arg:     m1
 */
__global__ void addMatrixSlow(float *m1, float *m2, uint64_t size){
    for (uint64_t i = 0; i < size; ++i){
        m1[i] = m1[i] + m2[i];
    }
}



/*<! Class for the assignment */
class MatVec{
public:
    /*--------------------------------------------------------------------------
     * Function:    Constructor
     * Purpose:     Constructs the class, sets constant variables and 
     *              initializes srand.
     *
     * In arg:      size
     */
    MatVec(uint64_t size) : n(size){
        srand (time(NULL));
        bytes = n * sizeof(float);
        ptr_bytes = n * sizeof(float*);        // Just being safe
        bytes_sqrd = bytes * bytes;
    }



    /*--------------------------------------------------------------------------
     * Function:    Destructor
     * Purpose:     Cleans up/frees allocated memory.
     *
     */
    ~MatVec(){
        freeAll();
    }



    /*--------------------------------------------------------------------------
     * Function:    sanityCheckFast
     * Purpose:     Performs a sanity check on the fastDotVec and 
     *              matAddElementWise CUDA functions. Outputs pass or fail to 
     *              console.
     *
     */
    void sanityCheckFast(){
        allocateAll();
        expected = allocateMemory(bytes);
        // Set up identity matrix, sequential/expected vectors
        for (uint64_t i = 0; i < n; ++i){
            for (uint64_t j = 0; j < n; ++j){
                uint64_t pos = i * n + j;
                if (i == j){
                    h_matrix1[pos] = 1.0f;
                }
                else{
                    h_matrix1[pos] = 0.0f;
                }
            }
            h_vec[i] = i;
            expected[i] = i;
        }

        // Copy from host to device
        cudaMemcpy(d_matrix1, h_matrix1, bytes_sqrd, cudaMemcpyHostToDevice);
        cudaMemcpy(d_vec, h_vec, bytes, cudaMemcpyHostToDevice);

        // Perform calculations on device
        fastDotVec<<<n, n>>>(d_matrix1, d_matrix2, d_vec, n);
        cudaDeviceSynchronize();
        matAddElementWise<<<1, n>>>(d_matrix2, d_vec, n);
        cudaDeviceSynchronize();

        // Copy result back to host & free memory
        cudaMemcpy(h_vec, d_vec, bytes, cudaMemcpyDeviceToHost);

        if (resultCheck(expected, h_vec)){
            std::cout << "Fast sanity check PASSED!" << std::endl;
        }
        else{
            std::cout << "Fast sanity check FAILED!" << std::endl;
            
        }
        // printArray(h_vec);
        freeAll();
    }



    /*--------------------------------------------------------------------------
     * Function:    sanityCheckSlow
     * Purpose:     Performs a sanity check on the slowDotVec CUDA function. 
     *              Outputs pass or fail to console.
     *
     */
    void sanityCheckSlow(){
        freeAll();
        allocateAll();
        expected = allocateMemory(bytes);
        // Set up identity matrix, sequential/expected vectors
        for (uint64_t i = 0; i < n; ++i){
            for (uint64_t j = 0; j < n; ++j){
                uint64_t pos = i * n + j;
                if (i == j){
                    h_matrix1[pos] = 1.0f;
                }
                else{
                    h_matrix1[pos] = 0.0f;
                }
            }
            h_vec[i] = i;
            expected[i] = i;
        }


        cudaMemcpy(d_matrix1, h_matrix1, bytes_sqrd, cudaMemcpyHostToDevice);
        cudaMemcpy(d_vec, h_vec, bytes, cudaMemcpyHostToDevice);

        slowDotVec<<<1, 1>>>(d_matrix1, d_vec, d_vec2, n);
        cudaDeviceSynchronize();
        cudaMemcpy(h_vec, d_vec2, bytes, cudaMemcpyDeviceToHost);

        if (resultCheck(expected, h_vec)){
            std::cout << "Slow sanity check PASSED!" << std::endl;
        }
        else{
            std::cout << "Slow sanity check FAILED!" << std::endl;
            
        }
        // printArray(h_vec);
    }



    /*--------------------------------------------------------------------------
     * Function:    AdotBfast
     * Purpose:     Frees memory, then allocates memory because we don't know 
     *              the previous class state. Then fills the matrix and array 
     *              with helper function fillMatrixArray(). Copies data to the 
     *              device via CudaMemcpy, performs the CUDA functions, 
     *              synchronizes, then transfers the data back to the host. 
     *              Returns the result array.
     *
     * In arg:      m, arr
     *
     * return:      h_vec
     */
    float* AdotBfast(float *m = NULL, float *arr = NULL){
        freeAll();
        allocateAll();
        
        fillMatrixArray(m, arr);

        // Copy host to device
        cudaMemcpy(d_matrix1, h_matrix1, bytes_sqrd, cudaMemcpyHostToDevice);
        cudaMemcpy(d_vec, h_vec, bytes, cudaMemcpyHostToDevice);

        // Perform operation
        fastDotVec<<<n, n>>>(d_matrix1, d_matrix2, d_vec, n);
        cudaDeviceSynchronize();    // Just in case
        matAddElementWise<<<1, n>>>(d_matrix2, d_vec, n);
        cudaDeviceSynchronize();


        // Copy result back to host
        cudaMemcpy(h_vec, d_vec, bytes, cudaMemcpyDeviceToHost);

        return h_vec;
        
    }



    /*--------------------------------------------------------------------------
     * Function:    AdotBslow
     * Purpose:     Frees memory, then allocates memory because we don't know 
     *              the previous class state. Then fills the matrix and array 
     *              with helper function fillMatrixArray(). Copies data to the 
     *              device via CudaMemcpy, performs the CUDA functions, 
     *              synchronizes, then transfers the data back to the host. 
     *              Returns the result array.
     *
     * In arg:      m, arr
     *
     * return:      h_vec
     */
    float* AdotBslow(float *m = NULL, float *arr = NULL){
        freeAll();
        allocateAll();

        fillMatrixArray(m, arr);

        // Copy host to device
        cudaMemcpy(d_matrix1, h_matrix1, bytes_sqrd, cudaMemcpyHostToDevice);
        cudaMemcpy(d_vec, h_vec, bytes, cudaMemcpyHostToDevice);

        // Perform operation
        slowDotVec<<<1, 1>>>(d_matrix1, d_vec, d_vec2, n);
        cudaDeviceSynchronize();

        // Copy result back to host
        cudaMemcpy(h_vec, d_vec2, bytes, cudaMemcpyDeviceToHost);

        return h_vec;
    }



    /*--------------------------------------------------------------------------
     * Function:    AplusBfast
     * Purpose:     Frees memory, then allocates memory because we don't know 
     *              the previous class state. Then fills both matrices with 
     *              helper function fillMatrixArray(). Copies data to the device 
     *              via CudaMemcpy, performs the CUDA function, synchronizes, 
     *              then transfers the data back to the host. Returns the result 
     *              array.
     *
     * In arg:      m1, m2
     *
     * return:      h_matrix1
     */
    float* AplusBfast(float *m1 = NULL, float *m2 = NULL){
        freeAll();
        allocateAll();
        
        fillMatrixMatrix(m1, m2);

        // Copy host to device
        cudaMemcpy(d_matrix1, h_matrix1, bytes_sqrd, cudaMemcpyHostToDevice);
        cudaMemcpy(d_matrix2, h_matrix2, bytes_sqrd, cudaMemcpyHostToDevice);

        // Perform operation
        addMatrixFast<<<n, n>>>(d_matrix1, d_matrix2, n * n);
        cudaDeviceSynchronize();


        // Copy result back to host
        cudaMemcpy(h_matrix1, d_matrix1, bytes_sqrd, cudaMemcpyDeviceToHost);

        return h_matrix1;
    }



    /*--------------------------------------------------------------------------
     * Function:    AplusBslow
     * Purpose:     Frees memory, then allocates memory because we don't know 
     *              the previous class state. Then fills both matrices with 
     *              helper function fillMatrixArray(). Copies data to the device 
     *              via CudaMemcpy, performs the CUDA function, synchronizes, 
     *              then transfers the data back to the host. Returns the result 
     *              array.
     *
     * In arg:      m1, m2
     *
     * return:      h_matrix1
     */
    float* AplusBslow(float *m1 = NULL, float *m2 = NULL){
        freeAll();
        allocateAll();
        
        fillMatrixMatrix(m1, m2);

        // Copy host to device
        cudaMemcpy(d_matrix1, h_matrix1, bytes_sqrd, cudaMemcpyHostToDevice);
        cudaMemcpy(d_matrix2, h_matrix2, bytes_sqrd, cudaMemcpyHostToDevice);

        // Perform operation
        addMatrixSlow<<<1, 1>>>(d_matrix1, d_matrix2, n * n);
        cudaDeviceSynchronize();


        // Copy result back to host
        cudaMemcpy(h_matrix1, d_matrix1, bytes_sqrd, cudaMemcpyDeviceToHost);

        return h_matrix1;
    }



    /*--------------------------------------------------------------------------
     * Function:    resultCheck
     * Purpose:     Performs checks between two arrays of size n. Verifies they 
     *              are the same to within an epsilon.
     *
     * In arg:      arr1, arr2
     *
     * return:      bool
     */
    bool resultCheck(float *arr1, float *arr2){
        for (uint64_t i = 0; i < n; ++i){
            if (fabs(arr1[i] - arr2[i]) > EPSILON){            
                return false;
            }
        }
        return true;
    }



    /*--------------------------------------------------------------------------
     * Function:    printArray
     * Purpose:     Prints an array of size n.
     *
     * In arg:      arr
     */
    void printArray(float *arr){
        for (uint64_t i = 0; i < n; ++i){
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    const float EPSILON = 0.0001;   // For sanity check
    const uint64_t n;               // Size of matrix (n x n)
    uint64_t bytes;                 // Bytes needed for array mallocs
    uint64_t ptr_bytes;             // Pointer bytes needed for mallocs
    uint64_t bytes_sqrd;            // Bytes for matrix mallocs

    // Dynamic memory
    float *h_matrix1;       // To store "Matrix A" and result matrix
    float *h_matrix2;       // To store "Matrix B"
    float *h_vec;           // To store "Vector B" and result

    float *d_matrix1;       // To store the data from host
    float *d_matrix2;       // To store the multiplication that happens
    float *d_vec;           // To store the data from host and result
    float *d_vec2;          // To store the data from host and result
    float *expected;        // To store expected sanity check result



    /*--------------------------------------------------------------------------
     * Function:    allocateMemory
     * Purpose:     Allocates memory on the host. Returns a pointer to the 
     *              allocated memory.
     *
     * return:      float pointer
     */
    float* allocateMemory(uint64_t size){
        return (float*)malloc(size);
    }



    /*--------------------------------------------------------------------------
     * Function:    allocateCudaMemory
     * Purpose:     Allocates memory on the device.
     *
     * return:      float pointer
     */
    void allocateCudaMemory(float *arr, uint64_t size){
        cudaMalloc((float**)&arr, size);
    }



    /*--------------------------------------------------------------------------
     * Function:    allocateAll
     * Purpose:     Allocates all memory necessary for any function call in this 
     *              class.
     *
     */
    void allocateAll(){
        // Initialize all dynamic memory
        h_matrix1 = allocateMemory(ptr_bytes * ptr_bytes);
        h_matrix2 = allocateMemory(ptr_bytes * ptr_bytes);
        h_vec = allocateMemory(bytes);

        allocateCudaMemory(d_matrix1, bytes_sqrd);
        allocateCudaMemory(d_matrix2, bytes_sqrd);
        allocateCudaMemory(d_vec, bytes);
        allocateCudaMemory(d_vec2, bytes);
    }



    /*--------------------------------------------------------------------------
     * Function:    freeAll
     * Purpose:     Frees all memory allocated, if it is allocated. Sets 
     *              pointers to NULL so we know their state.
     *
     */
    void freeAll(){
        if (h_vec != NULL){
            delete h_vec;
            h_vec = NULL;
        }
        if (h_matrix1 != NULL){
            delete h_matrix1;
            h_matrix1 = NULL;
        }
        if (h_matrix2 != NULL){
            delete h_matrix2;
            h_matrix2 = NULL;
        }
        if (d_matrix1 != NULL){
            cudaFree(d_matrix1);
            d_matrix1 = NULL;
        }
        if (d_matrix2 != NULL){
            cudaFree(d_matrix2);
            d_matrix2 = NULL;
        }
        if (d_vec != NULL){
            cudaFree(d_vec);
            d_vec = NULL;
        }
        if (d_vec2 != NULL){
            cudaFree(d_vec2);
            d_vec2 = NULL;
        }
    }



    /*--------------------------------------------------------------------------
     * Function:    fillMatrixArray
     * Purpose:     Helper class that fills h_matrix1 and h_vec with random 
     *              numbers if there is no matrix AND array passed to this 
     *              function. Otherwise, it will assign m1 and arr to h_matrix1 
     *              and h_vec. The user must clear up any allocated memory on 
     *              their end.
     *
     * In arg:      m, arr
     */
    void fillMatrixArray(float *m = NULL, float *arr = NULL){
        if (m == NULL || arr == NULL){
            // Fill "Matrix A"
            for (uint64_t i = 0; i < n * n; ++i){
                h_matrix1[i] = rand() % 1000;
            }

            // Fill "Vector B"
            for (uint64_t i = 0; i < n; ++i){    
                h_vec[i] = rand() % 1000;
            }
        }
        else{
            h_matrix1 = m;
            h_vec = arr;
        }
    }



    /*--------------------------------------------------------------------------
     * Function:    fillMatrixMatrix
     * Purpose:     Helper class that fills h_matrix1 and h_matrix2 with random 
     *              numbers if there is no matrix1 AND matrix2 passed to this 
     *              function. Otherwise, it will assign m1 and m2 to h_matrix1 
     *              and h_matrix2. The user must clear up any allocated memory 
     *              on their end.
     *
     * In arg:      m, arr
     */
    void fillMatrixMatrix(float *m1 = NULL, float *m2 = NULL){
        if (m1 == NULL || m2 == NULL){
            // Fill "Matrix A"
            for (uint64_t i = 0; i < n * n; ++i){
                h_matrix1[i] = rand() % 1000;
            }

            // Fill "Matrix B"
            for (uint64_t i = 0; i < n * n; ++i){
                h_matrix2[i] = rand() % 1000;
            }
        }
        else{
            h_matrix1 = m1;
            h_matrix2 = m2;
        }
    }
};



/*------------------------------------------------------------------------------
     * Function:    main
     * Purpose:     Runs the necessary functions to satisfy programing 
     *              assignment #2. Checks to see if the user specified a random 
     *              matrix/vector size to start, otherwise runs with the default 
     *              size of 1024. Instantiates the MatVec class and calls all 
     *              functions necessary to demonstrate assignment completion.
     *
     * In arg:      argc, argv
     *
     * returns:     0, 1
     */
int main(int argc, char **argv){
    uint64_t n = 1024;
    if (argc > 1){
        try{
            n = atoll(argv[1]);
            if (n == 0){
                std::cout << "Invalid n. Applied default 1024" << std::endl;
                n = 1024;
            }
            if (n > 1024){
                std::cout << "Program quit. n must be less than 1025" << std::endl;
                return 1;
            }
        }
        catch(...){
            std::cout << "Program quit. Usage: \"prog2 #\"" << std::endl;
            return 1;
        }
    }

    // Test MatVec functionality
    MatVec test = MatVec(n);

    // Perform sanity checks
    test.sanityCheckFast();
    test.sanityCheckSlow();
    
    // Undergrad section
    test.AdotBfast();
    test.AdotBslow();

    // Grad section
    test.AplusBfast();
    test.AplusBslow();

    return 0;
}