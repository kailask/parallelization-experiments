#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
using namespace chrono;

#define SECONDS 1

// Pull in different mutex implementations
#if defined(CPP_MUTEX)
#include <mutex>

mutex m;
#define init(n)
#define lock(t) m.lock()
#define unlock(t) m.unlock()

#elif defined(FILTER_MUTEX)

#include "filter.h"
mutex m;
#define init(n) m.init(n);
#define lock(t) m.lock(t);
#define unlock(t) m.unlock(t);

#elif defined(BAKERY_MUTEX)

#include "bakery.h"
mutex m;
#define init(n) m.init(n);
#define lock(t) m.lock(t);
#define unlock(t) m.unlock(t);

#else
#error No mutex specified!
#endif

int total_number = 0;
int *histogram;

// runs for 1 second, locking and unlocking over and over
void test_mutex(int thread_id) {
    auto start = high_resolution_clock::now();
    int duration = 0.0;
    while (duration < SECONDS) {
        // Perform the lock and unlock
        lock(thread_id);

        total_number++;
        histogram[thread_id]++;

        unlock(thread_id);

        // Update the timer
        auto now = high_resolution_clock::now();
        duration = duration_cast<seconds>(now - start).count();
    }
}

int main(int argc, char *argv[]) {
    // Read the number of threads in from the command line
    int num_threads = 8;
    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }

    // Create a new array for the number of threads
    thread *thread_array = new thread[num_threads];
    init(num_threads);

    // Initialize the histogram
    histogram = new int[num_threads];
    for (int i = 0; i < num_threads; i++) {
        histogram[i] = 0;
    }

    // Launch the threads
    for (int i = 0; i < num_threads; i++) {
        thread_array[i] = thread(test_mutex, i);
    }

    // Join the threads
    for (int i = 0; i < num_threads; i++) {
        thread_array[i].join();
    }

    // Print stats
    cout << "total number: " << total_number << endl;

    for (int i = 0; i < num_threads; i++) {
        cout << i << ":histogram: " << histogram[i] << endl;
    }

    // cleanup the memory
    delete[] thread_array;
    delete[] histogram;
}