#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include "blqs.h"

constexpr int SIZE = 50000000;
double data[SIZE];

double cputime() {
    return (double)clock() / CLOCKS_PER_SEC;
}

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

int main() {
	double t0;

	printf("blqsort - sorting %d million doubles ...\n", SIZE / 1000000);
	srand(1);
	for (int i = 0; i < SIZE; i++) data[i] = rand() / 1024.0;
	t0 = cputime();
	blqs::sort(data, data + SIZE);
	printf("Random:\t%.2fs\n", cputime() - t0);
	prchksum();

	t0 = cputime();
	blqs::sort(data, data + SIZE);
	printf("Sorted:\t%.2fs\n", cputime() - t0);

	for (int i = 0; i < 10; i++) std::swap(data[rand() % (SIZE / 10)], data[rand() % (SIZE / 10)]);
	t0 = cputime();
	blqs::sort(data, data + SIZE);
	printf("Nearly sorted: %.2fs\n", cputime() - t0);

	for (int i = 0; i < SIZE; i++) data[i] = rand() % 1000;
	t0 = cputime();
	blqs::sort(data, data + SIZE);
	printf("Duplicates: %.2fs\n", cputime() - t0);

	printf("\n");
	printf("std::sort - sorting %d million doubles ...\n", SIZE / 1000000);
	srand(1);
	for (int i = 0; i < SIZE; i++) data[i] = rand() / 1024.0;
	t0 = cputime();
	std::sort(data, data + SIZE);
	printf("Random:\t%.2fs\n", cputime() - t0);
	prchksum();

	t0 = cputime();
	std::sort(data, data + SIZE);
	printf("Sorted:\t%.2fs\n", cputime() - t0);

	for (int i = 0; i < 10; i++) std::swap(data[rand() % (SIZE / 10)], data[rand() % (SIZE / 10)]);
	t0 = cputime();
	std::sort(data, data + SIZE);
	printf("Nearly sorted: %.2fs\n", cputime() - t0);

	for (int i = 0; i < SIZE; i++) data[i] = rand() % 1000;
	t0 = cputime();
	std::sort(data, data + SIZE);
	printf("Duplicates: %.2fs\n", cputime() - t0);

}
