#include <assert.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#define SIZE 1024
#define VALUE 1048576
#define NUM_THREADS 8

typedef int thread_id;

using namespace std;
using namespace std::chrono;

void sequential(volatile int *arr, int size) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < VALUE; j++) arr[i]++;
    }
}

void round_robin(volatile int *arr, int size, int num_threads, thread_id id) {
    for (int i = id; i < SIZE; i += num_threads) {
        for (int j = 0; j < VALUE; j++) arr[i]++;
    }
}

void chunked(volatile int *arr, int size, int num_threads, thread_id id) {
    int chunk = size / num_threads;
    for (int i = (chunk * id); i < (chunk * (id + 1)); i++) {
        for (int j = 0; j < VALUE; j++) arr[i]++;
    }
}

int main(int argc, char const *argv[]) {
    int *a = (int *)malloc(SIZE * sizeof(int));
    int *b = (int *)malloc(SIZE * sizeof(int));
    int *c = (int *)malloc(SIZE * sizeof(int));

    for (int i = 0; i < SIZE; i++) {
        a[i] = 0;
        b[i] = 0;
        c[i] = 0;
    }

    auto a_start = high_resolution_clock::now();
    sequential(a, SIZE);
    auto a_stop = high_resolution_clock::now();
    auto a_duration = duration_cast<nanoseconds>(a_stop - a_start);
    auto a_seconds = a_duration.count() / 1000000000.0;

    thread threads[NUM_THREADS];

    auto b_start = high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(round_robin, b, SIZE, NUM_THREADS, i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
    auto b_stop = high_resolution_clock::now();
    auto b_duration = duration_cast<nanoseconds>(b_stop - b_start);
    auto b_seconds = b_duration.count() / 1000000000.0;

    auto c_start = high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(chunked, c, SIZE, NUM_THREADS, i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
    auto c_stop = high_resolution_clock::now();
    auto c_duration = duration_cast<nanoseconds>(c_stop - c_start);
    auto c_seconds = c_duration.count() / 1000000000.0;

    for (int i = 0; i < SIZE; i++) {
        assert(a[i] == VALUE);
        assert(b[i] == VALUE);
        assert(c[i] == VALUE);
    }

    cout << "sequential loop time: " << a_seconds << endl;
    cout << "round robin loop time: " << b_seconds << endl;
    cout << "chunked loop time: " << c_seconds << endl;
    cout << "----" << endl;
    cout << "speedups:" << endl;
    cout << "round robin speedup over sequential: " << a_seconds / b_seconds << endl;
    cout << "chunked speedup over sequential: " << a_seconds / c_seconds << endl;
    cout << "chunked speedup over round robin: " << b_seconds / c_seconds << endl
         << endl;
    return 0;
}
