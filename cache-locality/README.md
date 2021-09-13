# Cache Locality

Comparison of different algorithms for matrix multiplication that can exploit spatial and temporal cache locality.

* To exploit spatial locality one of the matrices is transposed so that contiguous memory is accessed more frequently. This exploits the fact that cache lines are larger than a single array index.
* To exploit temporal locaity the matrices are divided into blocks and each block is calculated separately.

## Setup

### Dependencies

* C++

### Running

```
make;
./matmul.out
```