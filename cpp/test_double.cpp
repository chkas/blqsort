// SPDX-License-Identifier: MIT
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <algorithm>

#include "blqs.h"

constexpr int SIZE = 50000000;
double data[SIZE];

static auto time0 = std::chrono::high_resolution_clock::now();
double time() {
    auto t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t - time0;
    return diff.count();
}

int main() {
	double t0;

	printf("Sorting %d million doubles\n", SIZE / 1000000);
	srand(1);
	for (int i = 0; i < SIZE; i++) data[i] = rand() / 1024.0;
	printf("blqs::sort: "); fflush(stdout);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("%.2fs\n", time() - t0);
	srand(1);
	for (int i = 0; i < SIZE; i++) data[i] = rand() / 1024.0;
	printf("std::sort:  "); fflush(stdout);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("%.2fs\n", time() - t0);
}
