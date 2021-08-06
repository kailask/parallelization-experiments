# SPMD Loop Comparision

Performance comparision of different Same-Program Multiple-Data loops incrementing values in an array.

1. Sequential execution
2. Round-robin mapping of thread:index
3. Chunk array indexes for each thread. This eliminates false sharing that occurs with round-robin selection of array index since multiple threads simultaneously access the same cache line.

## Setup

### Dependencies

* C++14
* Clang

### Running

```
make;
./SPMD.out
```