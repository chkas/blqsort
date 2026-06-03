// SPDX-License-Identifier: MIT
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <algorithm>

#include "blqs.h"

constexpr int SIZE = 50000000;
double data[SIZE];

void prchksum() {
	uint32_t* p = (uint32_t*)(void*)data;
	size_t len = sizeof(data) / (sizeof(uint32_t));
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= p[i];
        hash *= 16777619u;
    }
	printf("Checksum: %x\n", hash);
}
static auto time0 = std::chrono::high_resolution_clock::now();
double time() {
    auto t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t - time0;
    return diff.count();
}

int main() {
	double t0;

	printf("blqsort - sorting %d million doubles ...\n", SIZE / 1000000);
	srand(1);
	for (int i = 0; i < SIZE; i++) data[i] = rand() / 1024.0;
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("Random:\t%.2fs\n", time() - t0);
	prchksum();

	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("Sorted:\t%.2fs\n", time() - t0);

	for (int i = 0; i < 10; i++) std::swap(data[rand() % (SIZE / 10)], data[rand() % (SIZE / 10)]);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("Nearly sorted: %.2fs\n", time() - t0);

	for (int i = 0; i < SIZE; i++) data[i] = rand() % 1000;
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("Duplicates: %.2fs\n", time() - t0);
	prchksum();

	printf("\n");
	printf("std::sort - sorting %d million doubles ...\n", SIZE / 1000000);
	srand(1);
	for (int i = 0; i < SIZE; i++) data[i] = rand() / 1024.0;
	t0 = time();
	std::sort(data, data + SIZE);
	printf("Random:\t%.2fs\n", time() - t0);
	prchksum();

	t0 = time();
	std::sort(data, data + SIZE);
	printf("Sorted:\t%.2fs\n", time() - t0);

	for (int i = 0; i < 10; i++) std::swap(data[rand() % (SIZE / 10)], data[rand() % (SIZE / 10)]);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("Nearly sorted: %.2fs\n", time() - t0);

	for (int i = 0; i < SIZE; i++) data[i] = rand() % 1000;
	t0 = time();
	std::sort(data, data + SIZE);
	printf("Duplicates: %.2fs\n", time() - t0);
	prchksum();

}
