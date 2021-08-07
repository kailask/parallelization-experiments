#include <math.h>

#include <atomic>
#include <thread>

#if defined(SYNCQUEUE)
#include "CQueueSync.h"
#else
#include "CQueue.h"
#endif

#define SIZE (1024 * 1024 * 8)

CQueue memory_to_trig;
CQueue trig_to_memory;

void memory_thread_func(float *a, int size) {
    for (int i = 0; i < size; i++) {
        float loaded = a[i];
        memory_to_trig.enq(loaded);
        float to_store = trig_to_memory.deq();
        a[i] = to_store;
    }
}

void trig_thread_func(int size) {
    for (int i = 0; i < size; i++) {
        float consume = memory_to_trig.deq();
        consume = cos(consume);
        trig_to_memory.enq(consume);
    }
}

int main() {
    float *a = new float[SIZE];
    for (int i = 0; i < SIZE; i++) {
        a[i] = 0.5 + i;
    }
    std::thread memory_thread(memory_thread_func, a, SIZE);
    std::thread trig_thread(trig_thread_func, SIZE);

    memory_thread.join();
    trig_thread.join();

    return 0;
}
