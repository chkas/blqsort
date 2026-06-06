// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BLQS_CMP(a, b) ((a) < (b))
#define BLQS_TYPE double
#include "blqsort.h"

#define SIZE 50000000
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
double ts(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main() {
	double t0;

	printf("blqsort - sorting %d million doubles ...\n", SIZE / 1000000);
	srand(1);
	for (int i = 0; i < SIZE; i++) data[i] = rand() / 1024.0;
	t0 = ts();
	blqsort(data, SIZE);
	printf("Random:\t%.2fs\n", ts() - t0);
	prchksum();

	t0 = ts();
	blqsort(data, SIZE);
	printf("Sorted:\t%.2fs\n", ts() - t0);

	for (int i = 0; i < SIZE / 10; i++) data[i] = rand() / 1024.0;
	t0 = ts();
	blqsort(data, SIZE);
	printf("Nearly sorted: %.2fs\n", ts() - t0);

	for (int i = 0; i < SIZE; i++) data[i] = rand() % 1000;
	t0 = ts();
	blqsort(data, SIZE);
	printf("Duplicates: %.2fs\n", ts() - t0);
	prchksum();
}
