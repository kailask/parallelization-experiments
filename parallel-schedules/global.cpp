#include <assert.h>

#include <atomic>
#include <thread>

#include "work.h"

std::atomic_int counter(0);
void parallel_mult(float* result, int* mult, int size, int tid, int num_threads) {
    for (int i = std::atomic_fetch_add(&counter, 1); i < size; i = std::atomic_fetch_add(&counter, 1)) {
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
    linear_work(mult, SIZE);

    std::thread threads[NUM_THREADS];
    for (size_t i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::thread(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
}
