#include <assert.h>

#include <atomic>
#include <iostream>
#include <thread>

#include "IOQueue.h"
#include "work.h"

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);
void parallel_enq(int size, int tid, int num_threads) {
    Q[tid].init(size);
    size_t work_size = size / num_threads;
    for (size_t i = work_size * tid; i < work_size * (tid + 1); i++) {
        Q[tid].enq(i);
    }
}

void parallel_mult(float* result, int* mult, int size, int tid, int num_threads) {
    for (int i = Q[tid].deq(); i >= 0; i = Q[tid].deq()) {
        float base = result[i];
        for (int w = 0; w < mult[i] - 1; w++) {
            result[i] = result[i] + base;
        }
    }
    std::atomic_fetch_add(&finished_threads, 1);

    int steal_thead = NUM_THREADS - tid;
    while (finished_threads.load() != NUM_THREADS) {
        steal_thead = (steal_thead - 1 >= 0) ? steal_thead - 1 : NUM_THREADS - 1;
        for (int i = Q[steal_thead].deq(); i >= 0; i = Q[steal_thead].deq()) {
            float base = result[i];
            for (int w = 0; w < mult[i] - 1; w++) {
                result[i] = result[i] + base;
            }
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
        threads[i] = std::thread(parallel_enq, SIZE, i, NUM_THREADS);
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::thread(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
}
