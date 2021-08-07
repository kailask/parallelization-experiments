# Mutex Implementations

Implementations for various types of mutexes in C++:

* Filter Lock
* Bakery Mutex
* Reader-Writer Lock allows multiple simultaneous readers since RAR conficts don't exist.
* Fair(er) RW Lock attempts to prevent writer starvation by forcing readers to wait if a writer is waiting.

## Setup

### Dependencies

* C++

### Running

[benchmark.cpp](benchmark.cpp) tests the C++ mutex, bakery mutex, and filter mutex.
```
make mutex;
./cpp_mutex.out;
./filter_mutex.out;
./bakery_mutex.out;
```

[benchmark-rw.cpp](benchmark-rw.cpp) tests the RW and Fair RW mutexes.
```
make rw_mutex;
./rw_mutex.out;
./fair_rw_mutex.out;
```

## Links

* [Bakery Algorithm](https://en.wikipedia.org/wiki/Lamport's_bakery_algorithm)
* [Peterson's Algorithm](https://en.wikipedia.org/wiki/Peterson's_algorithm)