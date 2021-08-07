#include <atomic>
#include <iostream>
#include <thread>
using namespace std;

#include "../barriers/SRBarrier.h"
barrier_object B;

#define TEST_ITERATIONS (1024 * 256)

int var0, var1;

atomic_int x(0);
atomic_int y(0);
void t0_function() {
    B.barrier(0);
    x.store(1, memory_order_relaxed);
    var0 = y.load(memory_order_relaxed);
}

void t1_function() {
    B.barrier(1);
    y.store(1, memory_order_relaxed);
    var1 = x.load(memory_order_relaxed);
}

int main() {
    int output0 = 0;
    int output1 = 0;
    int output2 = 0;
    int output3 = 0;
    B.init(2);

    for (int i = 0; i < TEST_ITERATIONS; i++) {
        x.store(0);
        y.store(0);

        thread t0(t0_function);
        thread t1(t1_function);

        t0.join();
        t1.join();

        if (var0 == 0 && var1 == 1) {
            output0++;
        } else if (var0 == 1 && var1 == 0) {
            output1++;
        } else if (var0 == 1 && var1 == 1) {
            output2++;
        }

        // This should be the relaxed behavior
        else if (var0 == 0 && var1 == 0) {
            output3++;
        }
    }

    // Complete the print out using your output instantiations
    cout << "histogram of different observations:" << endl;
    cout << "output0: var0 == 0 && var1 == 1 " << output0 << endl;
    cout << "output1: var0 == 1 && var1 == 0 " << output1 << endl;
    cout << "output2: var0 == 1 && var1 == 1 " << output2 << endl;
    cout << "output3: var0 == 0 && var1 == 0 " << output3 << endl
         << endl;
    cout << "relaxed behavior frequency: " << float(output3) / float(TEST_ITERATIONS) << endl;

    return 0;
}
