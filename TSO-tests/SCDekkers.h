#include <atomic>

class dekkers_mutex {
   public:
    dekkers_mutex() {
        turn.store(0);
    }

    void lock(int tid) {
        wants_to_enter[tid].store(true);
        while (wants_to_enter[1 - tid].load()) {
            if (turn.load() != tid) {
                wants_to_enter[tid].store(false);
                while (turn.load() != tid) {
                };
                wants_to_enter[tid].store(true);
            }
        }
    }

    void unlock(int tid) {
        turn.store(1 - tid);
        wants_to_enter[tid].store(false);
    }

   private:
    std::atomic<bool> wants_to_enter[2];
    std::atomic<int> turn;
};
