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
* [consistent.cpp](consistent.cpp) - uses C++ default memory model for atomic operations which guarantees sequential consistency.
* [relaxed.cpp](relaxed.cpp) - uses C++ relaxed memory model which imposes no order on operations, on a TSO architecture like x86 this means store/load can be reordered.

A simple Dekkers mutex implementation to synchronize a counter in 3 ways:
1. [SCDekkers.h](SCDekkers.h) - implementation using C++ sequentially consistent memory order.
2. [RDekkers.h](RDekkers.h) - implementation using C++ relaxed memory order.
3. [TSODekkers.h](TSODekkers.h) - C++ relaxed memory order augmented with memory fences to make it sequentially consistent on TSO architectures like x86.

## Setup

### Dependencies

* C++17

### Running

```
make litmus;
./consistent.out;
./relaxed.out
```

```
make dekkers;
./scdekkers.out;
./rdekkers.out;
./tsodekkers.out
```

## Links

* [C++ Memory Orders](https://en.cppreference.com/w/cpp/atomic/memory_order)
* [Dekker's Algorithm](https://en.wikipedia.org/wiki/Dekker's_algorithm)
