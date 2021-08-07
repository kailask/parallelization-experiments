#define SIZE (1024 * 1024)
#define NUM_THREADS (6)

void linear_work(int* work, int size) {
    for (int i = 0; i < size; i++) {
        int new_i = (i / 128) + 1;
        work[i] = new_i;
    }
}

void squared_work(int* work, int size) {
    for (int i = 0; i < size; i++) {
        int new_i = (i / (1024 * 32)) + 1;
        work[i] = new_i * new_i;
    }
}

void cubed_work(int* work, int size) {
    for (int i = 0; i < size; i++) {
        int new_i = i / (1024 * 32 * 2);
        work[i] = new_i * new_i * new_i;
    }
}
