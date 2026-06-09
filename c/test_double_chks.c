// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#define BLQS_CMP(a, b) ((a) < (b))
#define BLQS_TYPE double
#include "blqsort.h"

#define SIZE 50000000
double data[SIZE];

uint32_t state = 1;
uint32_t mrand(void) {
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}
void chksum_test(uint32_t expect) {
	uint32_t* p = (uint32_t*)(void*)data;
	size_t len = sizeof(data) / (sizeof(uint32_t));
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= p[i];
        hash *= 16777619u;
    }
	if (hash != expect) printf("Checksum error %x\n", hash);
}
double ts(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main() {
	double t0;

	printf("blqsort - sorting %d million doubles ...\n", SIZE / 1000000);
	for (int i = 0; i < SIZE; i++) data[i] = mrand() / 1024.0;
	t0 = ts();
	blqsort(data, SIZE);
	printf("Random:\t%.2fs\n", ts() - t0);
	chksum_test(0x8f244fed);

	t0 = ts();
	blqsort(data, SIZE);
	printf("Sorted:\t%.2fs\n", ts() - t0);

	for (int i = 0; i < SIZE / 10; i++) data[i] = mrand() / 1024.0;
	t0 = ts();
	blqsort(data, SIZE);
	printf("Nearly sorted: %.2fs\n", ts() - t0);

	for (int i = 0; i < SIZE; i++) data[i] = mrand() % 1000;
	t0 = ts();
	blqsort(data, SIZE);
	printf("Duplicates: %.2fs\n", ts() - t0);
	chksum_test(0x143129c5);
}
