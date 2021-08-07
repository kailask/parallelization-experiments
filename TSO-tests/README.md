# TSO Tests

Some simple tests for observing relaxed memory behaviors on a Total Store Order memory model like x86.
```
Thread 0:
store(x,1);
%var0 = load(y);

Thread 1:
store(y,1);
%var1 = load(x);

Check: can %var0 == %var1 == 0 at the end of the execution?
```
* [consistent.cpp](consistent.cpp) - uses C++ default memory model for atomic operations which guarantees sequential consistency
* [relaxed.cpp](relaxed.cpp) - uses C++ relaxed memory model which imposes no order on operations, on a TSO architecture like x86 this means store/load can be reordered

## Setup

### Dependencies

* C++17

### Running

```
make;
./consistent.out;
./relaxed.out
```

## Links

* [C++ Memory Orders](https://en.cppreference.com/w/cpp/atomic/memory_order)
