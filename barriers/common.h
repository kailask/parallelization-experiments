#include <random>
#define SIZE 1024 * 256
#define REPEATS (1024 * 8 + 1)

// From https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c/9324796
double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
