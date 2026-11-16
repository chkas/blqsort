// SPDX-License-Identifier: MIT
#include <cstdio>
#include <cstdlib>
#include <chrono>
uint32_t state = 1;
void mrand_init() {
	state = 1;
}
uint32_t mrand(void) {
	uint32_t x = state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	state = x;
	return x;
}

#include <algorithm>
#include "blqs.h"

constexpr int SIZE = 50000000;

double data_arr[SIZE];
double data_arr2[SIZE];
double* data = data_arr;
double* data2 = data_arr2;

void shuffle(int pre_sort) {
	for (int i = 0; i < SIZE; i++) data2[i] = 0.0 / 0.0;
	for (int i = 0; i < SIZE / pre_sort; i++) {
		double val;
		int r;
		do r = rand() % SIZE; while (data[r] != data[r]);
		val = data[r];
		data[r] = 0.0 / 0.0;
		do r = rand() % SIZE; while (data2[r] == data2[r]);
		data2[r] = val;
	}
	for (int i = 0, j = 0; i < SIZE; i++) {
		while (data2[j] == data2[j]) j++;
		if (data[i] == data[i]) data2[j] = data[i];
	}
	std::swap(data, data2);
}
/*
void shuffle(int pre_sort) {
	// would be simpler but changes data:
	 for (int i = 0; i < SIZE / pre_sort; i++)
		data[mrand() % SIZE] = mrand() / 1024.0;
*/
/*
void shuffle(int pre_sort) {
	// simpler, but not realistic
	for (int i = 0; i < SIZE / pre_sort / 2; i++) {
		std::swap(data[rand() % SIZE], data[rand() % SIZE]);
	}
}
*/
void chksum_test(uint32_t expect) {
	uint32_t* p = (uint32_t*)(void*)data;
	size_t len = SIZE * sizeof(double) / (sizeof(uint32_t));
	uint32_t hash = 2166136261u;
	for (size_t i = 0; i < len; i++) {
		hash ^= p[i];
		hash *= 16777619u;
	}
	if (hash != expect) printf("Checksum error 0x%x\n", hash);
}
double time() {
	static auto time0 = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = t - time0;
	return diff.count();
}

int main() {
	double t0;

	printf("blqsort - sorting %d million doubles ...\n", SIZE / 1000000);
	for (int i = 0; i < SIZE; i++) data[i] = mrand() / 1024.0;
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("Random: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	shuffle(10);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("90%% sorted: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	shuffle(100);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("99%% sorted: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	shuffle(1000);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("99.9%% sorted: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	mrand_init();
	for (int i = 0; i < SIZE; i++) data[i] = mrand() % 1000;
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("Duplicates: %.2fs\n", time() - t0);
	chksum_test(0x24b2a9c5);

	printf("\n");
// -------------------------------------------------------------

	mrand_init();
	printf("std::sort - sorting %d million doubles ...\n", SIZE / 1000000);
	for (int i = 0; i < SIZE; i++) data[i] = mrand() / 1024.0;
	t0 = time();
	std::sort(data, data + SIZE);
	printf("Random: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	shuffle(10);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("90%% sorted: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	shuffle(100);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("99%% sorted: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	shuffle(1000);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("99.9%% sorted: %.2fs\n", time() - t0);
	chksum_test(0x8f244fed);

	mrand_init();
	for (int i = 0; i < SIZE; i++) data[i] = mrand() % 1000;
	t0 = time();
	std::sort(data, data + SIZE);
	printf("Duplicates: %.2fs\n", time() - t0);
	chksum_test(0x24b2a9c5);
}
