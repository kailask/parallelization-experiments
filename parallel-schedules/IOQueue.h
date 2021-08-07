#include <atomic>

class IOQueue {
   public:
    IOQueue() {
    }

    void init(int size) {
        this->size = size;
        data = new int[size];
        head.store(0);
        tail.store(0);
    }

    void enq(int e) {
        int old_head = std::atomic_fetch_add(&head, 1);
        data[old_head] = e;
    }

    int deq() {
        if (head.load() == tail.load()) return -1;
        int old_tail = std::atomic_fetch_add(&tail, 1);
        return data[old_tail];
    }

   private:
    int size;
    int* data;
    std::atomic_int head;
    std::atomic_int tail;
};
