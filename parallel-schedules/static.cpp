#include <assert.h>

#include <thread>

#include "work.h"

void parallel_mult(float* result, int* mult, int size, int tid, int num_threads) {
    size_t chunk_size = size / num_threads;
    for (int i = chunk_size * tid; i < chunk_size * (tid + 1); i++) {
        float base = result[i];
        for (int w = 0; w < mult[i] - 1; w++) {
            result[i] = result[i] + base;
        }
    }
}

int main() {
    float* result_parallel = new float[SIZE];
    int* mult = new int[SIZE];
    for (int i = 0; i < SIZE; i++) {
        result_parallel[i] = i;
    }
    linear_work(mult, SIZE);  //could be squared or cubed

    std::thread threads[NUM_THREADS];
    for (size_t i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::thread(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
}