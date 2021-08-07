#include <atomic>

class IOQueue {
   public:
    IOQueue() {
    }

    // Use this function to initialize the queue to
    // the size that you need.
    void init(int size) {
        this->size = size;
        data = new int[size];
        head.store(0);
        tail.store(0);
    }

    // enqueue the element e into the queue
    void enq(int e) {
        int old_head = std::atomic_fetch_add(&head, 1);
        data[old_head] = e;
    }

    // return a value from the queue.
    // return -1 if there is no more values
    // from the queue.
    int deq() {
        if (head.load() == tail.load()) return -1;
        int old_tail = std::atomic_fetch_add(&tail, 1);
        return data[old_tail];
    }

    // return 32 values from the queue. The
    // values should be returned through
    // the array passed in.
    // return 0 if successful. return 1 if not
    // i.e. if the queue does not have 32 elements
    int deq_32(int ret[32]) {
        if (head.load() < tail.load() + 32) return -1;
        int old_tail = std::atomic_fetch_add(&tail, 32);
        for (size_t i = 0; i < 32; i++) {
            ret[i] = data[old_tail + i];
        }
        return 0;
    }

   private:
    int size;
    int* data;
    std::atomic_int head;
    std::atomic_int tail;
};
