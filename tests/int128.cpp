#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include "blqs.h"

constexpr int SIZE = 50000000;
__int128 data[SIZE];

double cputime() {
    return (double)clock() / CLOCKS_PER_SEC;
}
int main() {
    double t0;

    for (int i = 0; i < SIZE; i++) data[i] = rand();
    t0 = cputime();
    blqs::sort(data, data + SIZE);
    printf("blqs::sort %.2fs\n", cputime() - t0);

    for (int i = 0; i < SIZE; i++) data[i] = rand();
    t0 = cputime();
    std::sort(data, data + SIZE);
    printf("std::sort %.2fs\n", cputime() - t0);
}
