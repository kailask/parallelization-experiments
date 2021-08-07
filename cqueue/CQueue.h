#include <atomic>

#define CQUEUE_SIZE 2048

class CQueue {
   public:
    CQueue() {
        head.store(0);
        tail.store(0);
    }

    void enq(float e) {
        while (tail.load() == (head.load() + 1) % size()) {
        }
        buffer[head.load()].store(e);
        head.store((head.load() + 1) % size());
    }

    void enq_8(float e[8]) {
        while (((tail.load() > head.load()) && (tail.load() - head.load() < 9)) ||
               ((head.load() > tail.load()) && (size() - (head.load() - tail.load()) < 9))) {
        }
        size_t start = head.load();
        for (size_t i = 0; i < 8; i++) {
            buffer[(start + i) % size()].store(e[i]);
        }
        head.store((head.load() + 8) % size());
    }

    float deq() {
        while (tail.load() == head.load()) {
        }
        float e = buffer[tail.load()].load();
        tail.store((tail.load() + 1) % size());
        return e;
    }

    void deq_8(float e[8]) {
        while (((tail.load() > head.load()) && (size() - (tail.load() - head.load()) < 8)) ||
               ((head.load() > tail.load()) && ((head.load() - tail.load()) < 8)) || (head.load() == tail.load())) {
        }
        size_t start = tail.load();
        for (size_t i = 0; i < 8; i++) {
            e[i] = buffer[(start + i) % size()].load();
        }
        tail.store((tail.load() + 8) % size());
    }

    int size() {
        return CQUEUE_SIZE;
    }

   private:
    std::atomic_int head;
    std::atomic_int tail;
    std::atomic<float> buffer[CQUEUE_SIZE];
};
