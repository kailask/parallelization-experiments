#pragma once

#include <atomic>
#include <thread>
using namespace std;

class mutex {
   public:
    mutex() {
        // Implement me!
    }

    void init(int num_threads) {
        size = num_threads;
        level = new atomic_int[num_threads];
        victim = new atomic_int[num_threads];
        for (size_t i = 0; i < num_threads; i++) {
            level[i].store(0);
            victim[i].store(0);
        }
    }

    void lock(int thread_id) {
        for (size_t i = 1; i < size; i++) {
            level[thread_id].store(i);
            victim[i].store(thread_id);
            for (size_t j = 0; j < size; j++) {
                if (j == thread_id) continue;
                while (level[j].load() >= i && victim[i].load() == thread_id) {
                    this_thread::yield();
                };
            }
        }
    }

    void unlock(int thread_id) {
        level[thread_id] = 0;
    }

   private:
    atomic_int* level;
    atomic_int* victim;
    int size;
};
