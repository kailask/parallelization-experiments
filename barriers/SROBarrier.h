#include <atomic>
#include <thread>

class barrier_object {
   public:
    barrier_object() {
        // Probably don't need to do anything here.
    }

    void init(int num_threads) {
        count.store(num_threads);
        sense.store(true);
        thread_sense = new std::atomic<bool>[num_threads];
        size = num_threads;
    }

    void barrier(int tid) {
        int index = count.fetch_add(-1);
        bool my_sense = thread_sense[tid].load();
        if (index == 1) {
            count.store(size);
            sense.store(my_sense);
        } else {
            while (sense.load() != my_sense) {
                while (sense.load(std::memory_order_relaxed) != my_sense) {
                    std::this_thread::yield();
                }
            };
        }
        thread_sense[tid].store(!my_sense);
    }

   private:
    std::atomic<int> count;
    std::atomic<bool> sense;
    std::atomic<bool>* thread_sense;
    int size;
};
