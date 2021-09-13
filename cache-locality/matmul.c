#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SIZE 1024

volatile __uint64_t A[SIZE][SIZE];
volatile __uint64_t B[SIZE][SIZE];
volatile __uint64_t Bt[SIZE][SIZE];
volatile __uint64_t C[SIZE][SIZE];
volatile __uint64_t Ct[SIZE][SIZE];
volatile __uint64_t Cb[SIZE][SIZE];

void init(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE]) {
    int r, c;

    for (c = 0; c < SIZE; c++) {
        for (r = 0; r < SIZE; r++) {
            A[r][c] = rand();
            B[r][c] = rand();
        }
    }
}

void transpose(volatile __uint64_t M[][SIZE], volatile __uint64_t Mt[][SIZE]) {
    int r, c;

    for (c = 0; c < SIZE; c++) {
        for (r = 0; r < SIZE; r++) { Mt[c][r] = M[r][c]; }
    }
}

int verify(volatile __uint64_t C[][SIZE], volatile __uint64_t D[][SIZE]) {
    int r, c;

    for (c = 0; c < SIZE; c++) {
        for (r = 0; r < SIZE; r++) {
            if (C[r][c] != D[r][c]) {
                printf("error! %i %i\n", r, c);
                return -1;
            }
        }
    }
    return 0;
}

int printMatrix(volatile __uint64_t M[][SIZE]) {
    int r, c;

    for (r = 0; r < SIZE; r++) {
        printf("%i: [", r);
        for (c = 0; c < SIZE; c++) { printf("%lu ", M[r][c]); }
        printf("]\n");
    }
    return 0;
}

void matmul(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE]) {
    int rowA, colB, idx;

    for (rowA = 0; rowA < SIZE; rowA++) {
        for (colB = 0; colB < SIZE; colB++) {
            for (idx = 0; idx < SIZE; idx++) {
                C[rowA][colB] += A[rowA][idx] * B[idx][colB];
            }
        }
    }
}

// Multiply a matrix with a transposed matrix
void matmul_t(volatile __uint64_t A[][SIZE], volatile __uint64_t Bt[][SIZE]) {
    int rowA, rowB, idx;

    for (rowA = 0; rowA < SIZE; rowA++) {
        for (rowB = 0; rowB < SIZE; rowB++) {
            for (idx = 0; idx < SIZE; idx++) {
                Ct[rowA][rowB] += A[rowA][idx] * Bt[rowB][idx];
            }
        }
    }
}

// Multiply two matrix tiles
void matmulCell(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE],
                int ax, int ay, int bx, int by, int size) {
    int rowA, colB, idx;

    for (rowA = ay; rowA < size + ay; rowA++) {
        for (colB = bx; colB < size + bx; colB++) {
            for (idx = 0; idx < size; idx++) {
                Cb[rowA][colB] += A[rowA][idx + ax] * B[idx + by][colB];
            }
        }
    }
}

void matmul_b(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE],
              int blockSize) {
    int rowA, colB, idx;

    for (rowA = 0; rowA < SIZE; rowA += blockSize) {
        for (idx = 0; idx < SIZE; idx += blockSize) {
            for (colB = 0; colB < SIZE; colB += blockSize) {
                matmulCell(A, B, idx, rowA, colB, idx, blockSize);
            }
        }
    }
}

void multiplyTiledMatrix(volatile __uint64_t A[][SIZE],
                         volatile __uint64_t B[][SIZE], int blockSize) {
    double time_taken;
    clock_t t;

    t = clock();
    matmul_b(A, B, blockSize);
    t = clock() - t;
    time_taken = ((double)t) / CLOCKS_PER_SEC;  // in seconds

    verify(C, Cb);
    printf("Tile size %i - %f seconds\n", blockSize, time_taken);

    memset((__uint64_t **)Cb, 0, sizeof(__uint64_t) * SIZE * SIZE);
}

int main(int argc, char **argv) {
    clock_t t;
    double time_taken;

    init(A, B);

    memset((__uint64_t **)Ct, 0, sizeof(__uint64_t) * SIZE * SIZE);
    memset((__uint64_t **)C, 0, sizeof(__uint64_t) * SIZE * SIZE);
    memset((__uint64_t **)Cb, 0, sizeof(__uint64_t) * SIZE * SIZE);

    printf("Performing matrix multiplication...\n");

    // Compute correct result
    t = clock();
    matmul(A, B);
    t = clock() - t;
    time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("Unoptimized algorithm %f seconds to execute\n\n", time_taken);

    printf("Optimizing for spatial locality...\n");
    // Compute with transpose
    transpose(B, Bt);
    t = clock();
    matmul_t(A, Bt);
    t = clock() - t;

    verify(C, Ct);

    time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("Multiplication took %f seconds to execute\n\n", time_taken);

    // Compute with tiling
    printf("Optimizing for temporal locality...\n");
    if (argc == 2) {
        int tileSize = atoi(argv[1]);
        multiplyTiledMatrix(A, B, tileSize);
    } else {
        // If no tile size do all tile sizes
        for (int i = 0; i <= 10; i++) { multiplyTiledMatrix(A, B, pow(2, i)); }
    }

    return 0;
}
