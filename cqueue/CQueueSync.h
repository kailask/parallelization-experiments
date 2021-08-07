#include <atomic>

class CQueue {
   public:
    CQueue() {
        empty.store(false);
    }

    void enq(float e) {
        while (!empty.load()) {
        }
        value.store(e);
        empty.store(false);
    }

    float deq() {
        while (empty.load()) {
        }
        float e = value.load();
        empty.store(true);
        return e;
    }

   private:
    std::atomic_bool empty;
    std::atomic_int value;
};
