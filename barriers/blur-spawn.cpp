#include <chrono>
#include <iostream>
#include <thread>

#include "common.h"

void blur(double *input, double *output, int size, int tid, int num_threads) {
    int chunk_size = size / num_threads;
    for (int i = tid * chunk_size; i < (tid + 1) * chunk_size; i++) {
        if (i == 0 || i == size - 1) continue;
        output[i] = (input[i] + input[i + 1] + input[i - 1]) / 3;
    }
}

int main(int argc, char *argv[]) {
    int num_threads = 8;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    double *input = new double[SIZE];
    double *output = new double[SIZE];

    for (int i = 0; i < SIZE; i++) {
        double randval = fRand(-100.0, 100.0);
        input[i] = randval;
        output[i] = randval;
    }

    std::thread threads[num_threads];

    auto time_start = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < REPEATS; r++) {
        for (size_t i = 0; i < num_threads; i++) {
            threads[i] = std::thread(blur, input, output, SIZE, i, num_threads);
        }

        for (size_t i = 0; i < num_threads; i++) {
            threads[i].join();
        }

        double *tmp = input;
        input = output;
        output = tmp;
    }

    auto time_end = std::chrono::high_resolution_clock::now();
    auto time_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
    double time_seconds = time_duration.count() / 1000000000.0;
    std::cout << "timings: " << time_seconds << std::endl;
}