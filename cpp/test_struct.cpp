// SPDX-License-Identifier: MIT
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <algorithm>

#include "blqs.h"

constexpr int SIZE = 50000000;

struct entry {
	int id;
	int value;
	bool operator<(const entry& other) const {
		return id < other.id;
	}
};

struct entry data[SIZE];

double time() {
	static auto time0 = std::chrono::high_resolution_clock::now();
    auto t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t - time0;
    return diff.count();
}

void init() {
	for (int i = 0; i < SIZE; i++) {
		data[i].id = rand();
		data[i].value = i;
	}
}
int main() {
	double t0;
	printf("Sorting %d million structs ...\n", SIZE / 1000000);

	init();
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("blqs::sort: %.2fs\n", time() - t0);

	init();
	t0 = time();
	std::sort(data, data + SIZE);
	printf("std::sort: %.2fs\n", time() - t0);
}

