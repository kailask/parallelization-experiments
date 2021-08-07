# Barrier Implementations

Implementations for synchronizing a group of threads performing a repeated blur operation on an array.
```
void repeated_blur(double *input, double *output, int size, int repeats) {
    for (int r = 0; r < repeats; r++) {
        for (int i = 1; i < size - 1; i++) {
            output[i] = (input[i] + input[i+1] + input[i-1])/3
        }
        double *tmp = input;
        input = output;
        output = tmp;
    }
}
```
1. Spawn new group of threads for each iteration.
2. Use sense reversal barrier to synchronize threads after each iteration - sense reversal is needed so a thread can determine which side of the barrier its on.
3. Optimized sense reversal barrier - threads use relaxed peeking and yield if waiting, this performs better on over-subscribed systems.

### Dependencies

* C++17

### Running

```
make;
time ./sjbarrier.out 6;
time ./srbarrier.out 6;
time ./srobarrier.out 6
```
