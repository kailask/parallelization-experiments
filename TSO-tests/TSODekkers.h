#include <atomic>

#define FENCE asm volatile("mfence":::"memory");

class dekkers_mutex {
   public:
    dekkers_mutex() {
        turn.store(0, std::memory_order_relaxed);
    }

    void lock(int tid) {
        wants_to_enter[tid].store(true, std::memory_order_relaxed);
        FENCE
        while (wants_to_enter[1 - tid].load(std::memory_order_relaxed)) {
            if (turn.load(std::memory_order_relaxed) != tid) {
                wants_to_enter[tid].store(false, std::memory_order_relaxed);
                while (turn.load(std::memory_order_relaxed) != tid) {
                };
                wants_to_enter[tid].store(true, std::memory_order_relaxed);
                FENCE
            }
        }
    }

    void unlock(int tid) {
        turn.store(1 - tid, std::memory_order_relaxed);
        wants_to_enter[tid].store(false, std::memory_order_relaxed);
    }

   private:
    std::atomic<bool> wants_to_enter[2];
    std::atomic<int> turn;
};