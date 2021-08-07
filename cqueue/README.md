# Concurrent Queue

Comparison of synchronous and asynchronous concurrent queue implementations. Asynchronous concurrent queue uses a circular buffer to allow multiple elements to be enqueued or dequeued asynchronously.

## Setup

### Dependencies

* C++14
* Clang

### Running

```
make;
time ./syncQueue.out;
time ./asyncQueue.out
```

## Links

* [FreeBSD buf_ring](http://fxr.watson.org/fxr/source/sys/buf_ring.h?v=FREEBSD-10-4)

