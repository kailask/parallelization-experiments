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
        flag = new atomic_bool[num_threads];
        label = new atomic_int[num_threads];
        for (size_t i = 0; i < num_threads; i++) {
            flag[i].store(false);
            label[i].store(0);
        }
    }

    void lock(int thread_id) {
        flag[thread_id].store(true);
        int max = 0;
        for (size_t i = 0; i < size; i++) {
            int l = label[i].load();
            if (l > max) max = l;
        }
        int my_label = max + 1;
        label[thread_id].store(my_label);

        for (size_t i = 0; i < size; i++) {
            if (i == thread_id) continue;
            int l = label[i].load();
            while (flag[i].load() == true && ((l < my_label) || (l == my_label && i < thread_id))) {
                this_thread::yield();
                l = label[i].load();
            };
        }
    }

    void unlock(int thread_id) {
        flag[thread_id].store(false);
    }

   private:
    int size;
    atomic_bool* flag;
    atomic_int* label;
};
