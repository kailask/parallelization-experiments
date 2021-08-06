#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
using namespace chrono;

#define SECONDS 1

// Pull in different mutex implementations
#if defined(DEFAULT_MUTEX)
#include "rw.h"

#elif defined(FAIR_MUTEX)

#include "fair-rw.h"

#else
#error No mutex specified!
#endif

rw_mutex m;

atomic_int total_readers(0);
atomic_int total_writers(0);

#define READERS 6
#define WRITERS 2

// runs for 1 second, locking and unlocking in the reader state over
// and over
void reader() {
    auto start = high_resolution_clock::now();
    int duration = 0.0;
    while (duration < SECONDS) {
        atomic_fetch_add(&total_readers, 1);

        // Perform the lock and unlock
        m.lock_reader();
        m.unlock_reader();

        // Update the timer
        auto now = high_resolution_clock::now();
        duration = duration_cast<seconds>(now - start).count();
    }
}

// runs for 1 second, locking and unlocking in the writer (default)
// state over and over
void writer() {
    auto start = high_resolution_clock::now();
    int duration = 0.0;
    while (duration < SECONDS) {
        atomic_fetch_add(&total_writers, 1);

        // Perform the lock and unlock
        m.lock();
        m.unlock();

        // Update the timer
        auto now = high_resolution_clock::now();
        duration = duration_cast<seconds>(now - start).count();
    }
}

int main(int argc, char *argv[]) {
    // Create a new array for the number of threads
    thread *reader_ar = new thread[READERS];
    thread *writer_ar = new thread[WRITERS];

    // Launch the reader threads
    for (int i = 0; i < READERS; i++) {
        reader_ar[i] = thread(reader);
    }

    // Launch the writer threads
    for (int i = 0; i < WRITERS; i++) {
        writer_ar[i] = thread(writer);
    }

    // Join the reader threads
    for (int i = 0; i < READERS; i++) {
        reader_ar[i].join();
    }

    // Join the writer threads
    for (int i = 0; i < WRITERS; i++) {
        writer_ar[i].join();
    }

    // Print stats
    cout << "total readers: " << total_readers << endl;
    cout << "total writers: " << total_writers << endl;
}
