// SPDX-License-Identifier: MIT
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <algorithm>

#include "blqs.h"

constexpr int SIZE = 50000000;
double data[SIZE];

uint32_t mrand(void) {
	return rand();
}
long long seed;
void mrand_init() {
	seed = time(NULL);
	srand(seed);
}
void order_test() {
	for (int i = 1; i < SIZE; i++) {
		if (data[i - 1] > data[i]) {
			printf("Order error -  seed %lld\n", seed);
			exit(1);
		}
	}
}

double time() {
	static auto time0 = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = t - time0;
	return diff.count();
}

int main() {
	double t0;

	for (int round = 0; round < 10000; round++) {
		printf("Round %d\n", round);
		mrand_init();

		for (int i = 0; i < SIZE; i++) data[i] = mrand() / 1024.0;
		blqs::sort(data, data + SIZE);
		order_test();
	
		for (int i = 0; i < SIZE / 100; i++) {
			std::swap(data[mrand() % SIZE], data[mrand() % SIZE]);
		}
		blqs::sort(data, data + SIZE);
		order_test();

		for (int i = 0; i < SIZE / 2; i++) {
			std::swap(data[i], data[SIZE - i - 1]);
		}
		for (int i = 0; i < SIZE / 100; i++) {
			std::swap(data[mrand() % SIZE], data[mrand() % SIZE]);
		}
		blqs::sort(data, data + SIZE);
		order_test();
	
		for (int i = 0; i < SIZE; i++) data[i] = mrand() % 1000;
		blqs::sort(data, data + SIZE);
		order_test();
	}
	printf("\n");
}
